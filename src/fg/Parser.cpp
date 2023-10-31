#include "fg/Parser.hpp"

#include "fg/Lexer.hpp"
#include "fg/rules.hpp"

#include "regex/Parser.hpp"
#include "utils/Error.hpp"

#include <unordered_map>
#include <sstream>

namespace parsec::fg {
	namespace {
		class ParseImpl : AtomVisitor {
		public:
			/** @{ */
			explicit ParseImpl(std::istream& input) noexcept
				: m_lexer(input)
			{ }
			/** @} */


			/** @{ */
			Grammar operator()() {
				parseGrammar();
				
				// construct a grammar from the parsed symbol definitions
				Grammar grammar;
				for(auto& [name, rule] : m_symbols) {
					// check the rule for any undefined symbol references and then add it to the grammar
					if(!rule.second) { // skip terminal symbols
						rule.first->traverse(*this);
					}
					grammar.putSymbol(name, std::move(rule.first), rule.second);
				}
				return grammar;
			}
			/** @} */


		private:
			/** @{ */
			void visit(const Atom& n) override {
				const auto it = m_symbols.find(n.value());
				if(it == m_symbols.cend()) {
					throw Error(
						"reference to an undefined symbol \"" + n.value() + '"',
						m_atomLocs[&n]
					);
				}
			}
			/** @} */


			/** @{ */
			[[noreturn]]
			void unexpectedToken() const {
				throw Error("unexpected \""
						+ m_lexer.peek().text()
						+ '"',
					m_lexer.peek().loc()
				);
			}

			[[noreturn]]
			void unmatchedParen(const SourceLoc& loc) const {
				throw Error(
					"unmatched parenthesis",
					loc
				);
			}
			/** @} */


			/** @{ */
			RulePtr parseRegex() {
				const auto pattern = m_lexer.expect(TokenKinds::StringLiteral);
				try {
					// parse the regex into its rule representation
					return regex::Parser().parse(pattern.text());
				} catch(const Error& e) {
					// adjust the error location to take into account the relative position of the regex
					throw Error(e.what(), {
						pattern.loc().startCol() + e.loc().startCol() + 1,
						e.loc().colCount(),
						pattern.loc().lineNo(),
						pattern.loc().linePos()
					});
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
						const auto tok = m_lexer.lex();
						auto atom = makeRule<Atom>(
							tok.text()
						);
						
						m_atomLocs.emplace(static_cast<const Atom*>(
							atom.get()), tok.loc()
						);
						return atom;
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
			void addSymbol(const std::string& name, RulePtr rule, bool terminal) {
				const auto [it, wasInserted] = m_symbols.try_emplace(name);
				if(wasInserted) {
					// the new symbol is inserted as is
					it->second = { std::move(rule), terminal };
				} else {
					// combine the rules defined by the same name into one symbol
					it->second.first = makeRule<RuleAltern>(
						std::move(it->second).first, std::move(rule)
					);
				}
			}

			void parseTokensList() {
				m_lexer.expect(TokenKinds::OpenBrace);
				while(!m_lexer.skipIf(TokenKinds::CloseBrace)) {
					// terminal symbols are defined with "name = pattern;"
					const auto name = m_lexer.expect(TokenKinds::Ident);
					m_lexer.expect(TokenKinds::Equals);

					addSymbol(name.text(), parseRegex(), true);
					m_lexer.expect(TokenKinds::Semicolon);
				}
			}

			void parseRulesList() {
				m_lexer.expect(TokenKinds::OpenBrace);
				while(!m_lexer.skipIf(TokenKinds::CloseBrace)) {
					// nonterminal symbols are defined with "name = rule;"
					const auto name = m_lexer.expect(TokenKinds::Ident);
					m_lexer.expect(TokenKinds::Equals);

					addSymbol(name.text(), parseRule(), false);
					m_lexer.expect(TokenKinds::Semicolon);
				}
			}

			void parseGrammar() {
				while(!m_lexer.peek().eof()) {
					if(m_lexer.skipIf("tokens")) {
						parseTokensList();
					} else if(m_lexer.skipIf("rules")) {
						parseRulesList();
					} else {
						unexpectedToken();
					}
				}
			}
			/** @} */


			/** @{ */
			std::unordered_map<const Atom*, SourceLoc> m_atomLocs;
			std::unordered_map<std::string, std::pair<RulePtr, bool>> m_symbols;
			Lexer m_lexer;
			/** @} */
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