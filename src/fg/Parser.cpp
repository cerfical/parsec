#include "fg/Parser.hpp"

#include "fg/Lexer.hpp"
#include "fg/rules.hpp"

#include "regex/Parser.hpp"
#include "utils/ParseError.hpp"

#include <sstream>

namespace parsec::fg {
	namespace {
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
				if(const auto symbol = m_grammar.lookupSymbol(name.text())) {
					symbolRedefinition(*symbol, name.loc());
				}
				m_lexer.expect(TokenKinds::Equals);

				m_grammar.putSymbol(name.text(), parseRegex(), true, name.loc());
			}

			void parseRuleDef() {
				// syntax rules are of the form: name = rule
				const auto name = m_lexer.expect(TokenKinds::Ident);
				if(const auto symbol = m_grammar.lookupSymbol(name.text())) {
					symbolRedefinition(*symbol, name.loc());
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
		return ParseImpl(input)();
	}
}