#include "fg/Parser.hpp"

#include "fg/Lexer.hpp"
#include "fg/rules.hpp"

#include "regex/Parser.hpp"
#include "utils/Error.hpp"

#include <unordered_map>
#include <format>
#include <sstream>

namespace parsec::fg {
	namespace {
		class ParseImpl : RuleTraverser {
		public:
			explicit ParseImpl(std::istream& input) noexcept
				: m_lexer(input)
			{ }

			Grammar operator()() {
				parseGrammar();
				
				// check the grammar for undefined symbols
				for(const auto sym : m_grammar.nonterminals()) {
					m_currentEnd = sym->rule()->endAtom();
					traverse(*sym->rule());
				}

				return std::move(m_grammar);
			}

		private:
			/** @{ */
			void visit(const Atom& n) override {
				// skip end atoms as a special case
				if(&n == m_currentEnd) {
					return;
				}

				if(!m_grammar.symbolByName(n.value())) {
					throw Error(
						std::format("reference to an undefined symbol \"{}\"", n.value()),
						m_atomLocs[&n]
					);
				}
			}
			/** @} */


			/** @{ */
			[[noreturn]]
			void unexpectedToken() const {
				throw Error(
					std::format(
						"unexpected \"{}\"",
						m_lexer.peek().text()
					),
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
			
			[[noreturn]]
			void symbolRedefinition(const Token& name) const {
				const auto sym = m_grammar.symbolByName(name.text());
				throw Error(
					std::format("redifinition of \"{}\"", sym->name()),
					name.loc()
				);
			}
			/** @} */


			/** @{ */
			bool isAtom() const {
				switch(m_lexer.peek().kind()) {
					case TokenKinds::Ident:
					case TokenKinds::OpenParen:
					case TokenKinds::StringLiteral:
					case TokenKinds::RegularExpr: {
						return true;
					}
					default: {
						return false;
					}
				}
			}
			/** @} */


			/** @{ */
			RulePtr parseRegex(const Token& pattern) {
				try {
					// parse the regex into its rule representation
					return regex::Parser(
						pattern.kind() == TokenKinds::StringLiteral ?
							regex::ParseOptions::NoRegexSyntax : regex::ParseOptions::None
					).parse(pattern.text());
				} catch(const Error& e) {
					// adjust the error location to take into account regex position
					throw Error(e.what(), {
						pattern.loc().startCol() + e.loc().startCol() + 1,
						e.loc().colCount(),
						pattern.loc().lineNo(),
						pattern.loc().linePos()
					});
				}
			}
			
			RulePtr parseRegex() {
				switch(m_lexer.peek().kind()) {
					case TokenKinds::StringLiteral:
					case TokenKinds::RegularExpr: {
						return parseRegex(m_lexer.lex());
					}
					default: {
						unexpectedToken();
					}
				}
			}
			/** @} */


			/** @{ */
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
					case TokenKinds::StringLiteral:
					case TokenKinds::RegularExpr: {
						const auto tokPattern = m_lexer.lex();
						const auto tokName = std::format(
							"<{0}{1}{0}>",
							tokPattern.kind() == TokenKinds::RegularExpr ? '"' : '\'',
							tokPattern.text()
						);

						m_grammar.addTerminal(
							tokName, parseRegex(tokPattern)
						);

						return makeRule<Atom>(tokName);
					}
					default: {
						unexpectedToken();
					}
				}
			}

			RulePtr parseConcat() {
				auto lhs = parseAtom();
				while(isAtom()) {
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
			void addSymbol(const Token& name, RulePtr rule, bool terminal) {
				if(terminal
					&& !m_grammar.addTerminal(name.text(), std::move(rule)) || !terminal
					&& !m_grammar.addNonterminal(name.text(), std::move(rule))
				) {
					symbolRedefinition(name);
				}
			}

			void parseTokensList() {
				m_lexer.expect(TokenKinds::OpenBrace);
				while(!m_lexer.skipIf(TokenKinds::CloseBrace)) {
					// terminal symbols are defined with "name = pattern;"
					const auto name = m_lexer.expect(TokenKinds::Ident);
					m_lexer.expect(TokenKinds::Equals);

					addSymbol(name, parseRegex(), true);
					m_lexer.expect(TokenKinds::Semicolon);
				}
			}

			void parseRulesList() {
				m_lexer.expect(TokenKinds::OpenBrace);
				while(!m_lexer.skipIf(TokenKinds::CloseBrace)) {
					// nonterminal symbols are defined with "name = rule;"
					const auto name = m_lexer.expect(TokenKinds::Ident);
					m_lexer.expect(TokenKinds::Equals);

					addSymbol(name, parseRule(), false);
					m_lexer.expect(TokenKinds::Semicolon);
				}
			}

			void parseGrammar() {
				while(!m_lexer.isEof()) {
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


			std::unordered_map<const Atom*, SourceLoc> m_atomLocs;
			Lexer m_lexer;

			const Atom* m_currentEnd = nullptr;
			Grammar m_grammar;
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