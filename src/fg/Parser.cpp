#include "fg/Parser.hpp"

#include "fg/Lexer.hpp"
#include "fg/rules.hpp"

#include "regex/Parser.hpp"
#include "utils/ParseError.hpp"

#include <algorithm>
#include <sstream>

namespace ranges = std::ranges;

namespace parsec::fg {
	namespace {
		class FindStartSymbol : RuleTraverser {
		public:
			/** @{ */
			explicit FindStartSymbol(const Grammar& grammar) noexcept
				: m_grammar(grammar)
			{ }
			/** @} */


			/** @{ */
			const Symbol* operator()() {
				// find all symbols that have no references
				m_startSymbols.resize(m_grammar.symbols().size(), true);
				for(const auto& sym : m_grammar.symbols()) {
					if(!sym.terminal()) {
						m_currentSymbol = &sym;
						sym.ruleBody()->traverse(*this);
					} else {
						// terminal symbols cannot be start symbols
						m_startSymbols[sym.id()] = false;
					}
				}

				// no start symbol, it is okay
				const auto it = ranges::find(m_startSymbols, true);
				if(it == m_startSymbols.cend()) {
					return nullptr;
				}

				// only one start symbol is defined, return it
				if(ranges::count(it + 1, m_startSymbols.cend(), true) == 0) {
					const auto symId = ranges::distance(m_startSymbols.cbegin(), it);
					return &m_grammar.symbols()[symId];
				}

				// no single entry point to the grammar was found
				ambiguousStart();
			}
			/** @} */


		private:
			/** @{ */
			static ParseError makeError(const Symbol& sym) {
				return ParseError(
					"ambiguous start symbol \"" + sym.name() + '"',
					sym.loc()
				);
			}
			/** @} */


			/** @{ */
			void ambiguousStart(std::size_t i) const {
				// find the next candidate for the start symbol, if any
				while(true) {
					// base case for recursion
					if(i == m_startSymbols.size()) {
						return;
					}
					if(m_startSymbols[i]) {
						break;
					}
					i++;
				}

				// recursive case
				const auto err = makeError(m_grammar.symbols()[i]);
				try {
					ambiguousStart(i + 1);
				} catch(...) {
					std::throw_with_nested(err);
				}
				throw err;
			}
			
			[[noreturn]] void ambiguousStart() const {
				ambiguousStart(0);
			}
			/** @} */


			/** @{ */
			void visit(const Atom& n) override {
				const auto sym = m_grammar.lookupSymbol(n.value());
				if(sym && sym != m_currentSymbol) {
					m_startSymbols[sym->id()] = false;
				}
			}

			void visit(const NilRule& n) override {
				// nothing to do
			}

			void visit(const PlusRule& n) override {
				n.inner()->traverse(*this);
			}

			void visit(const StarRule& n) override {
				n.inner()->traverse(*this);
			}

			void visit(const OptionalRule& n) override {
				n.inner()->traverse(*this);
			}

			void visit(const RuleAltern& n) override {
				n.left()->traverse(*this);
				n.right()->traverse(*this);
			}

			void visit(const RuleConcat& n) override {
				n.left()->traverse(*this);
				n.right()->traverse(*this);
			}
			/** @} */


			/** @{ */
			const Symbol* m_currentSymbol = nullptr;
			std::vector<bool> m_startSymbols;
			const Grammar& m_grammar;
			/** @} */
		};

		class ParseImpl {
		public:
			/** @{ */
			explicit ParseImpl(std::istream& input) noexcept
				: m_lexer(input)
			{ }
			/** @} */


			/** @{ */
			Grammar operator()() {
				return (parseGrammar(), std::move(m_grammar));
			}
			/** @} */


		private:
			/** @{ */
			[[noreturn]]
			void unexpectedToken() const {
				throw ParseError("unexpected \""
						+ m_lexer.peek().text()
						+ '"',
					m_lexer.peek().loc()
				);
			}

			[[noreturn]]
			void unmatchedParen(const SourceLoc& loc) const {
				throw ParseError(
					"unmatched parenthesis",
					loc
				);
			}

			[[noreturn]]
			void symbolRedefinition(const Symbol& sym, const SourceLoc& loc) {
				// attach additional info to the parse error using nested exceptions
				try {
					throw ParseError("previous definition of \""
							+ sym.name()
							+ "\" was here",
						sym.loc()
					);
				} catch(...) {
					std::throw_with_nested(ParseError(
						"redifinition of \"" + sym.name() + '"',
						loc
					));
				}
			}
			/** @} */


			/** @{ */
			RulePtr parseRegex() {
				const auto pattern = m_lexer.expect(TokenKinds::StringLiteral);
				try {
					// parse the regex into its rule representation
					return regex::Parser().parse(pattern.text());
				} catch(const ParseError& e) {
					// adjust the error location to take into account the relative position of the regex
					throw ParseError(e.what(),
						SourceLoc(
							pattern.loc().startCol() + e.loc().startCol() + 1,
							e.loc().colCount(),
							pattern.loc().lineNo(),
							pattern.loc().linePos()
						)
					);
				}
			}
			/** @} */


			/** @{ */
			bool atomStart() const {
				switch(m_lexer.peek().kind()) {
					case TokenKinds::Ident: return true;
					case TokenKinds::OpenParen: return true;
					default: return false;
				}
			}

			RulePtr parseAtom() {
				switch(m_lexer.peek().kind()) {
					case TokenKinds::Ident: {
						return makeRule<Atom>(
							m_lexer.lex().text()
						);
					}
					case TokenKinds::OpenParen: {
						const auto openParen = m_lexer.lex();
						
						auto e = parseRule();
						if(!m_lexer.skipIf(TokenKinds::CloseParen)) {
							unmatchedParen(openParen.loc());
						}
						return e;
					}
					default: {
						unexpectedToken();
					}
				}
			}

			RulePtr parseConcat() {
				auto lhs = parseAtom();
				while(atomStart()) {
					lhs = makeRule<RuleConcat>(
						std::move(lhs),
						parseAtom()
					);
				}
				return lhs;
			}

			RulePtr parseAltern() {
				auto lhs = parseConcat();
				while(m_lexer.skipIf(TokenKinds::Pipe)) {
					lhs = makeRule<RuleAltern>(
						std::move(lhs),
						parseConcat()
					);
				}
				return lhs;
			}

			RulePtr parseRule() {
				return parseAltern();
			}
			/** @} */


			/** @{ */
			void parseTokenDef() {
				// token definitions are of the form: name = "pattern"
				const auto name = m_lexer.expect(TokenKinds::Ident);
				if(const auto sym = m_grammar.lookupSymbol(name.text())) {
					symbolRedefinition(*sym, name.loc());
				}
				m_lexer.expect(TokenKinds::Equals);

				m_grammar.putSymbol(name.text(), parseRegex(), true, name.loc());
			}

			void parseRuleDef() {
				// syntax rules are of the form: name = rule
				const auto name = m_lexer.expect(TokenKinds::Ident);
				if(const auto sym = m_grammar.lookupSymbol(name.text())) {
					symbolRedefinition(*sym, name.loc());
				}
				m_lexer.expect(TokenKinds::Equals);
				
				m_grammar.putSymbol(name.text(), parseRule(), false, name.loc());
			}

			void parseDefList() {
				const auto listType = m_lexer.expect(TokenKinds::Ident);
				m_lexer.expect(TokenKinds::OpenBrace);

				while(!m_lexer.skipIf(TokenKinds::CloseBrace)) {
					if(listType.text() == "tokens") {
						parseTokenDef();
					} else if(listType.text() == "rules") {
						parseRuleDef();
					} else {
						m_lexer.skip();
						continue;
					}
					m_lexer.expect(TokenKinds::Semicolon);
				}
			}

			void parseGrammar() {
				while(!m_lexer.peek().eof()) {
					parseDefList();
				}
			}
			/** @} */


			/** @{ */
			Grammar m_grammar;
			Lexer m_lexer;
			/** @} */
		};
	}


	Grammar Parser::parse(std::string_view input) {
		auto in = std::istringstream(std::string(input));
		return parse(in);
	}

	Grammar Parser::parse(std::istream& input) {
		auto grammar = ParseImpl(input)();
		grammar.setStartSymbol(FindStartSymbol(grammar)());
		return grammar;
	}
}