#include "fg/Parser.hpp"
#include "fg/Lexer.hpp"

#include "fg/RuleTraverser.hpp"
#include "fg/rules.hpp"

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
				for(const auto sym : m_grammar.rules()) {
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
			RulePtr parseRegex() {
				return nullptr;
			}
			
			RulePtr parseUnnamedToken() {
				const auto& patternToken = m_lexer.peek();
				const auto pattern = std::format("{0}{1}{0}",
					patternToken.kind() == TokenKinds::RegularExpr ? '"' : '\'',
					patternToken.text()
				);

				const auto fakeNameId = m_unnamedTokenFakeNames.size();
				const auto [it, wasInserted] = m_unnamedTokenFakeNames.try_emplace(pattern);
				auto& fakeName = it->second;
				if(wasInserted) {
					fakeName = std::format("Unnamed{}_", fakeNameId);
				}

				const auto tok = m_grammar.insertToken(fakeName, parseRegex());
				return makeRule<Atom>(tok->name());
			}
			/** @} */


			/** @{ */
			RulePtr parseAtom() {
				switch(m_lexer.peek().kind()) {
					case TokenKinds::Ident: {
						const auto ident = m_lexer.lex();
						auto atom = makeRule<Atom>(
							ident.text()
						);
						
						m_atomLocs.emplace(static_cast<const Atom*>(
							atom.get()), ident.loc()
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
						return parseUnnamedToken();
					}
					default: {
						unexpectedToken();
					}
				}
			}

			RulePtr parseRepeat() {
				auto r = parseAtom();

				while(true) {
					if(m_lexer.skipIf(TokenKinds::Star)) {
						const auto name = std::format("_{}{}", m_ruleSymbolName, m_subruleId);
						m_grammar.insertRule(name,
							makeRule<OptionalRule>(
								makeRule<RuleConcat>(
									makeRule<Atom>(name),
									std::move(r)
								)
							)
						);
						m_subruleId++;

						r = makeRule<Atom>(name);
						//r = makeRule<StarRule>(std::move(r));
					} else if(m_lexer.skipIf(TokenKinds::Plus)) {
						const auto name = std::format("_{}{}", m_ruleSymbolName, m_subruleId);
						m_grammar.insertRule(name,
							makeRule<RuleConcat>(
								makeRule<OptionalRule>(
									makeRule<Atom>(name)
								),
								std::move(r)
							)
						);
						m_subruleId++;
						
						r = makeRule<Atom>(name);
					} else if(m_lexer.skipIf(TokenKinds::Qo)) {
						r = makeRule<OptionalRule>(std::move(r));
					} else {
						break;
					}
				}

				return r;
			}

			RulePtr parseConcat() {
				auto lhs = parseRepeat();
				while(isAtom()) {
					lhs = makeRule<RuleConcat>(
						std::move(lhs),
						parseRepeat()
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
			void parseTokenList() {
				m_lexer.expect(TokenKinds::OpenBrace);
				while(!m_lexer.skipIf(TokenKinds::CloseBrace)) {
					// tokens are defined with "token-name = token-regex-pattern;"
					const auto name = m_lexer.expect(TokenKinds::Ident);
					m_lexer.expect(TokenKinds::Equals);
					
					// parse and store the token definition in the grammar
					switch(m_lexer.peek().kind()) {
						case TokenKinds::StringLiteral:
						case TokenKinds::RegularExpr: {
							if(!m_grammar.insertToken(name.text(), parseRegex())) {
								symbolRedefinition(name);
							}
							break;
						}
						default: {
							unexpectedToken();
						}
					}

					m_lexer.expect(TokenKinds::Semicolon);
				}
			}

			void parseRuleList() {
				m_lexer.expect(TokenKinds::OpenBrace);
				while(!m_lexer.skipIf(TokenKinds::CloseBrace)) {
					// rules are defined with "rule-name = rule;"
					const auto name = m_lexer.expect(TokenKinds::Ident);
					m_lexer.expect(TokenKinds::Equals);
					
					// parse and store the rule in the grammar
					m_ruleSymbolName = name.text();
					m_subruleId = 0;
					if(!m_grammar.insertRule(name.text(), parseRule())) {
						symbolRedefinition(name);
					}

					m_lexer.expect(TokenKinds::Semicolon);
				}
			}

			void parseGrammar() {
				while(!m_lexer.isEof()) {
					if(m_lexer.skipIf("tokens")) {
						parseTokenList();
					} else if(m_lexer.skipIf("rules")) {
						parseRuleList();
					} else {
						unexpectedToken();
					}
				}
			}
			/** @} */


			std::string m_ruleSymbolName;
			int m_subruleId = 0;

			std::unordered_map<std::string, std::string> m_unnamedTokenFakeNames;
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