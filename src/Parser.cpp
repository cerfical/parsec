#include "Parser.hpp"
#include "Lexer.hpp"
#include "Error.hpp"

#include "regex/Parser.hpp"
#include "regex/NodeVisitor.hpp"
#include "regex/nodes.hpp"

#include <sstream>
#include <unordered_map>
#include <ranges>
#include <format>
#include <stack>
#include <list>

namespace parsec {
	namespace {
		class IsNullable : regex::NodeVisitor {
		public:
			bool operator()(const regex::ExprNode& n) noexcept {
				n.acceptVisitor(*this);
				return m_nullable;
			}

		private:
			void visit(const regex::CharAtom&) override {
				m_nullable = false;
			}

			void visit(const regex::NilExpr&) override {
				m_nullable = true;
			}


			void visit(const regex::OptionalExpr&) override {
				m_nullable = true;
			}

			void visit(const regex::PlusExpr&) override {
				m_nullable = false;
			}

			void visit(const regex::StarExpr&) override {
				m_nullable = true;
			}


			void visit(const regex::AlternExpr& n) override {
				n.left()->acceptVisitor(*this); // check if the left child is nullable
				if(!m_nullable) { // if it is not, check the right child
					n.right()->acceptVisitor(*this);
				}
			}

			void visit(const regex::ConcatExpr& n) override {
				n.left()->acceptVisitor(*this); // check if the left child is nullable
				if(m_nullable) { // if it is, check the right child
					n.right()->acceptVisitor(*this);
				}
			}


			bool m_nullable = false;
		};

		class FindRoots : regex::NodeVisitor {
		public:
			auto operator()(const regex::ExprNode& n) {
				n.acceptVisitor(*this);
				return std::move(m_roots);
			}

		private:
			/** @{ */
			void visit(const regex::CharAtom& n) override {
				m_roots.push_back(&n);
			}

			void visit(const regex::NilExpr&) override {
				// nothing to do
			}


			void visit(const regex::OptionalExpr& n) override {
				visitUnary(n);
			}

			void visit(const regex::PlusExpr& n) override {
				visitUnary(n);
			}

			void visit(const regex::StarExpr& n) override {
				visitUnary(n);
			}


			void visit(const regex::AlternExpr& n) override {
				// add roots of the left child
				n.left()->acceptVisitor(*this);
				// add roots of the right child
				n.right()->acceptVisitor(*this);
			}

			void visit(const regex::ConcatExpr& n) override {
				// add roots of the left child
				n.left()->acceptVisitor(*this);
				if(IsNullable()(*n.left())) {
					// add roots of the right child
					n.right()->acceptVisitor(*this);
				}
			}
			/** @} */


			/** @{ */
			void visitUnary(const regex::UnaryExpr& n) {
				// add roots of the inner expression
				n.inner()->acceptVisitor(*this);
			}
			/** @} */


			std::vector<const regex::CharAtom*> m_roots;
		};

		class FindFollows : regex::NodeVisitor {
		public:
			auto operator()(const regex::ExprNode& n) {
				n.acceptVisitor(*this);
				return std::move(m_atoms);
			}

		private:
			/** @{ */
			void visit(const regex::CharAtom& n) override {
				traverseParent(n);
			}

			void visit(const regex::NilExpr&) override {
				// nothing to do
			}


			void visit(const regex::OptionalExpr& n) override {
				traverseParent(n);
			}

			void visit(const regex::PlusExpr& n) override {
				appendRoots(n);
				traverseParent(n);
			}

			void visit(const regex::StarExpr& n) override {
				appendRoots(n);
				traverseParent(n);
			}


			void visit(const regex::AlternExpr& n) override {
				traverseParent(n);
			}

			void visit(const regex::ConcatExpr& n) override {
				if(m_child == n.left()) {
					appendRoots(*n.right());
					if(IsNullable()(*n.right())) {
						traverseParent(n);
					}
				} else {
					traverseParent(n);
				}
			}
			/** @} */


			/** @{ */
			void traverseParent(const regex::ExprNode& n) {
				// recursively traverse the parent node to find all atoms following the given one
				if(n.parent()) {
					const auto oldChild = std::exchange(m_child, &n);
					n.parent()->acceptVisitor(*this);
					m_child = oldChild;
				}
			}

			void appendRoots(const regex::ExprNode& n) {
				const auto atoms = FindRoots()(n);
				m_atoms.insert(m_atoms.end(),
					atoms.cbegin(),
					atoms.cend()
				);
			}
			/** @} */


			std::vector<const regex::CharAtom*> m_atoms;
			const regex::ExprNode* m_child = nullptr;
		};

		class FindEndAtoms : regex::NodeVisitor {
		public:
			/** @{ */
			auto operator()(const regex::ExprNode& n) {
				n.acceptVisitor(*this);
				return std::move(m_endAtoms);
			}
			/** @} */


		private:
			/** @{ */
			void visit(const regex::CharAtom& n) override {
				m_endAtoms.push_back(&n);
			}

			void visit(const regex::NilExpr&) override {
				// nothing to do
			}


			void visit(const regex::OptionalExpr& n) override {
				// add end atoms of the inner expression
				n.inner()->acceptVisitor(*this);
			}

			void visit(const regex::PlusExpr& n) override {
				// add end atoms of the inner expression
				n.inner()->acceptVisitor(*this);
			}

			void visit(const regex::StarExpr& n) override {
				// add end atoms of the inner expression
				n.inner()->acceptVisitor(*this);
			}


			void visit(const regex::AlternExpr& n) override {
				// add end atoms of the left child
				n.left()->acceptVisitor(*this);
				// add end atoms of the right child
				n.right()->acceptVisitor(*this);
			}

			void visit(const regex::ConcatExpr& n) override {
				// add end atoms of the right child
				n.right()->acceptVisitor(*this);
				if(IsNullable()(*n.right())) {
					// add end atoms of the left child
					n.left()->acceptVisitor(*this);
				}
			}
			/** @} */


			std::vector<const regex::CharAtom*> m_endAtoms;
		};



		class ParseImpl {
		public:
			explicit ParseImpl(std::istream& input) noexcept
				: m_lexer(input)
			{ }

			Grammar operator()() {
				parseGrammar();

				// handle all unnamed patterns that had not been linked to some named token
				std::ranges::for_each(std::views::values(m_patternToSymbols)
					| std::views::filter([](const auto& t) { return !t.first->pattern(); }),
					[this, unnamedId = 0](const auto& t) mutable {
						// construct a fake symbol for the unnamed pattern
						const auto patternHead = compilePattern(std::format("Unnamed{}_", unnamedId), t.second);
						unnamedId++;

						// insert the fake symbol into all places where the unnamed pattern occurs
						m_grammar.replaceSymbol(t.first, patternHead);
					}
				);

				return std::move(m_grammar);
			}

		private:
			/** @{ */
			[[noreturn]] void unexpectedToken() const {
				const auto& tok = m_lexer.peek();
				throw Error(std::format("unexpected \"{}\"",
						tok.text()
					), tok.loc()
				);
			}

			[[noreturn]] void unmatchedParen(const SourceLoc& loc) const {
				throw Error("unmatched parenthesis", loc);
			}
			/** @} */


			/** @{ */
			bool isAtom() const {
				switch(m_lexer.peek().kind()) {
					case TokenKinds::Ident:
					case TokenKinds::OpenParen:
					case TokenKinds::Pattern: {
						return true;
					}
					default: {
						return false;
					}
				}
			}
			/** @} */


			/** @{ */
			const Symbol* compilePattern(const std::string& name, const Token& patternToken) {
				auto head = m_grammar.symbolForName(name);
				auto pattern = m_grammar.insertPattern(head);
				auto regex = parseRegex(patternToken);

				// keep a mapping between regex atoms and pattern atoms
				std::unordered_map<const regex::CharAtom*, CharAtom*> regexToAtoms;
				std::stack<const regex::CharAtom*> unprocessed;
				
				// first, find all root atoms of the regex and mark them as "unprocessed"
				for(const auto regexRoot : FindRoots()(*regex)) {
					const auto root = pattern->addRoot(regexRoot->value());
					regexToAtoms[regexRoot] = root;
					unprocessed.push(regexRoot);
				}

				// then iteratively find all atoms, that follow some other atom
				while(!unprocessed.empty()) {
					const auto regexAtom = unprocessed.top();
					auto atom = regexToAtoms[regexAtom];
					unprocessed.pop();

					for(const auto regexFollow : FindFollows()(*regexAtom)) {
						const auto [it, wasInserted] = regexToAtoms.try_emplace(regexFollow, nullptr);
						auto& follow = it->second;
						
						if(wasInserted) {
							follow = pattern->addAtom(regexFollow->value());
							unprocessed.push(regexFollow);
						}

						atom->addFollow(follow);
					}
				}

				// mark the pattern end
				for(const auto regexEnd : FindEndAtoms()(*regex)) {
					auto endAtom = regexToAtoms[regexEnd];
					endAtom->addFollow(pattern->endAtom());
				}

				if(IsNullable()(*regex)) {
					pattern->markRoot(pattern->endAtom());
				}

				return head;
			}
			
			regex::ExprPtr parseRegex(const Token& pattern) {
				try {
					return regex::Parser().parse(pattern.text());
				} catch(const Error& e) {
					// adjust the error location to take into account the location of the pattern token
					throw Error(e.what(), {
						pattern.loc().startCol() + e.loc().startCol() + 1,
						e.loc().colCount(),
						pattern.loc().lineNo(),
						pattern.loc().linePos()
					});
				}
			}

			const Symbol* makeSubruleSymbol(const Rule& rule) {
				return m_grammar.symbolForName(std::format("{}{}_",
					rule.head()->name(),
					m_subruleId++
				));
			}
			/** @} */


			/** @{ */
			const Symbol* parseUnnamedPattern() {
				auto patternToken = m_lexer.lex();

				// check for the presence of a token with a similar pattern
				const auto [it, wasInserted] = m_patternToSymbols.try_emplace(patternToken.text());
				auto& [symbol, token] = it->second;
				
				// if such a token has not yet been defined, then
				if(wasInserted) {
					// mark the the place where the pattern was found
					symbol = &m_unnamedMarkers.emplace_back("", 0);
					token = std::move(patternToken);
				}
				
				return symbol;
			}

			const Symbol* parseAtom(const Rule& rule) {
				switch(m_lexer.peek().kind()) {
					case TokenKinds::Ident: {
						const auto ident = m_lexer.lex();
						return m_grammar.symbolForName(
							ident.text()
						);
					}
					case TokenKinds::OpenParen: {
						// create a fake symbol to represent the inner subrule
						const auto subruleHead = makeSubruleSymbol(rule);
						const auto openParen = m_lexer.lex();

						// parse the subrule, if there is any
						if(!m_lexer.skipIf(TokenKinds::CloseParen)) {
							parseRule(subruleHead);

							// check for the presence of parenthesis closing the subrule
							if(!m_lexer.skipIf(TokenKinds::CloseParen)) {
								unmatchedParen(openParen.loc());
							}
						} else {
							// otherwise, just insert an empty rule
							m_grammar.insertRule(subruleHead);
						}
						return subruleHead;
					}
					case TokenKinds::Pattern: {
						return parseUnnamedPattern();
					}
					case TokenKinds::CloseParen: {
						unmatchedParen(m_lexer.peek().loc());
					}
					default: {
						unexpectedToken();
					}
				}
			}

			void parseRepeat(Rule& rule) {
				auto subruleHead = parseAtom(rule);
				while(true) {
					if(m_lexer.skipIf(TokenKinds::Star)) {
						// rule: head = head atom
						auto atom = std::exchange(subruleHead, makeSubruleSymbol(rule));
						(*m_grammar.insertRule(subruleHead))
							.addSymbol(subruleHead)
							.addSymbol(atom);

						// empty rule
						m_grammar.insertRule(subruleHead);
					} else if(m_lexer.skipIf(TokenKinds::Plus)) {
						// rule: head = head atom
						auto atom = std::exchange(subruleHead, makeSubruleSymbol(rule));
						(*m_grammar.insertRule(subruleHead))
							.addSymbol(subruleHead)
							.addSymbol(atom);

						// rule: head = atom
						(*m_grammar.insertRule(subruleHead))
							.addSymbol(atom);
					} else if(m_lexer.skipIf(TokenKinds::Qo)) {
						// rule: head = atom
						auto atom = std::exchange(subruleHead, makeSubruleSymbol(rule));
						subruleHead = makeSubruleSymbol(rule);
						(*m_grammar.insertRule(subruleHead))
							.addSymbol(atom);

						// empty rule
						m_grammar.insertRule(subruleHead);
					} else {
						rule.addSymbol(subruleHead);
						break;
					}
				}
			}

			void parseConcat(Rule& rule) {
				parseRepeat(rule);
				while(isAtom()) {
					parseRepeat(rule);
				}
			}

			void parseRule(const Symbol* head) {
				parseConcat(*m_grammar.insertRule(head));
				while(m_lexer.skipIf(TokenKinds::Pipe)) {
					parseConcat(*m_grammar.insertRule(head));
				}
			}
			/** @} */


			/** @{ */
			void parseTokenList() {
				m_lexer.match(TokenKinds::OpenBrace);
				while(!m_lexer.skipIf(TokenKinds::CloseBrace)) {
					// tokens are defined with "token-name = token-pattern;"
					const auto name = m_lexer.expect(TokenKinds::Ident);
					m_lexer.match(TokenKinds::Equals);
					
					// parse and store the token definition in the grammar
					const auto patternToken = m_lexer.expect(TokenKinds::Pattern);
					const auto patternHead = compilePattern(name.text(), patternToken);
					
					const auto [it, wasInserted] = m_patternToSymbols.try_emplace(patternToken.text());
					auto& oldSymbol = it->second.first;

					if(wasInserted) {
						// the pattern is unique
						oldSymbol = patternHead;
					} else if(!oldSymbol->pattern()) {
						// the pattern had already been encountered as unnamed token
						m_grammar.replaceSymbol(oldSymbol, patternHead);
					}

					m_lexer.match(TokenKinds::Semicolon);
				}
			}

			void parseRuleList() {
				m_lexer.match(TokenKinds::OpenBrace);
				while(!m_lexer.skipIf(TokenKinds::CloseBrace)) {
					// rules are defined with "rule-name = rule;"
					const auto name = m_lexer.expect(TokenKinds::Ident);
					m_lexer.match(TokenKinds::Equals);
					
					// parse and store the rule in the grammar
					m_subruleId = 0;
					parseRule(m_grammar.symbolForName(name.text()));
					
					m_lexer.match(TokenKinds::Semicolon);
				}
			}

			void parseGrammar() {
				while(!m_lexer.isEof()) {
					if(m_lexer.skipIf("Tokens")) {
						parseTokenList();
					} else if(m_lexer.skipIf("Rules")) {
						parseRuleList();
					} else {
						unexpectedToken();
					}
				}
			}
			/** @} */


			std::unordered_map<std::string, std::pair<const Symbol*, Token>> m_patternToSymbols;
			std::list<Symbol> m_unnamedMarkers;

			int m_subruleId = 0;

			Grammar m_grammar;
			Lexer m_lexer;
		};
	}


	Grammar Parser::parse(std::string_view input) {
		auto in = std::istringstream(std::string(input));
		return parse(in);
	}

	Grammar Parser::parse(std::istream& input) {
		return ParseImpl(input)();
	}
}