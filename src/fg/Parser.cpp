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
			[[noreturn]] void unexpectedToken() const {
				throw ParseError("unexpected \""
						+ m_lexer.peek().text()
						+ '"',
					m_lexer.peek().loc()
				);
			}

			[[noreturn]] void unmatchedParen(const SourceLoc& loc) const {
				throw ParseError(
					"unmatched parenthesis",
					loc
				);
			}

			[[noreturn]] void duplicateName() {
				throw ParseError(
					"duplicate grammar symbol name",
					m_lexer.peek().loc()
				);
			}
			/** @} */


			/** @{ */
			RulePtr parseRegex(const Token& tok) {
				try {
					// parse the regex into its rule representation
					return regex::Parser().parse(tok.text());
				} catch(const ParseError& e) {
					// adjust the error location to take into account the relative position of the regex
					throw ParseError(e.what(),
						SourceLoc(
							tok.loc().startCol() + e.loc().startCol() + 1,
							e.loc().colCount(),
							tok.loc().lineNo(),
							tok.loc().linePos()
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
				// token definition is of the form: name = "pattern";
				const auto name = m_lexer.expect(TokenKinds::Ident);
				if(m_grammar.lookupSymbol(name.text())) {
					duplicateName();
				}
				m_lexer.expect(TokenKinds::Equals);

				// store the parsed regex pattern in the grammar
				const auto pattern = m_lexer.expect(TokenKinds::StringLiteral);
				m_grammar.putSymbol(name.text(), parseRegex(pattern));

				m_lexer.expect(TokenKinds::Semicolon);
			}

			void parseTokenDefList() {
				m_lexer.expect(TokenKinds::OpenBrace);

				while(m_lexer.peek().is(TokenKinds::Ident)) {
					parseTokenDef();
				}

				m_lexer.expect(TokenKinds::CloseBrace);
			}

			void parseRuleDef() {
				// syntax rule are of the form: name = rule;
				const auto name = m_lexer.expect(TokenKinds::Ident).text();
				if(m_grammar.lookupSymbol(name)) {
					duplicateName();
				}

				m_lexer.expect(TokenKinds::Equals);
				m_grammar.putSymbol(name, parseRule(), false);

				m_lexer.expect(TokenKinds::Semicolon);
			}

			void parseRuleDefList() {
				m_lexer.expect(TokenKinds::OpenBrace);

				while(m_lexer.peek().is(TokenKinds::Ident)) {
					parseRuleDef();
				}

				m_lexer.expect(TokenKinds::CloseBrace);
			}

			void parseGrammar() {
				while(!m_lexer.peek().eof()) {
					if(m_lexer.skipIf("tokens")) {
						parseTokenDefList();
					} else if(m_lexer.skipIf("rules")) {
						parseRuleDefList();
					} else {
						unexpectedToken();
					}
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