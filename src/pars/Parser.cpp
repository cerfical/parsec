#include "pars/Parser.hpp"

#include "pars/Lexer.hpp"
#include "pars/ast.hpp"

#include "err.hpp"

#include <sstream>

namespace parsec::pars {
	namespace {
		class ParserImpl {
		public:

			explicit ParserImpl(std::istream& input)
				: m_lexer(input) {}

			NodePtr run() {
				return parseSpec();
			}

		private:
			NodePtr parseSpec() {
				auto spec = makeNode<EmptyNode>();
				while(!m_lexer.isEof()) {
					if(m_lexer.skipIf("tokens")) {
						spec = makeNode<ListNode>(std::move(spec), parseTokenList());
					} else if(m_lexer.skipIf("rules")) {
						spec = makeNode<ListNode>(std::move(spec), parseRuleList());
					} else {
						const auto& tok = m_lexer.peek();
						err::misplacedToken(tok.loc(), tok.text());
					}
				}
				return spec;
			}


			NodePtr parseTokenList() {
				auto tokens = makeNode<EmptyNode>();
				expect<TokenKinds::LeftBrace>();

				while(!m_lexer.skipIf(TokenKinds::RightBrace)) {
					const auto name = expect<TokenKinds::Ident>();
					expect<TokenKinds::Equals>();

					auto pattern = makeNode<NamedToken>(name, expect<TokenKinds::PatternString>());
					expect<TokenKinds::Semicolon>();

					tokens = makeNode<ListNode>(
						std::move(tokens),
						std::move(pattern)
					);
				}

				return tokens;
			}


			NodePtr parseRuleList() {
				auto rules = makeNode<EmptyNode>();
				expect<TokenKinds::LeftBrace>();

				while(!m_lexer.skipIf(TokenKinds::RightBrace)) {
					const auto name = expect<TokenKinds::Ident>();
					expect<TokenKinds::Equals>();

					auto rule = makeNode<NamedRule>(
						name,
						parseRule()
					);
					expect<TokenKinds::Semicolon>();

					rules = makeNode<ListNode>(
						std::move(rules),
						std::move(rule)
					);
				}

				return rules;
			}


			NodePtr parseRule() {
				return parseAltern();
			}


			NodePtr parseAltern() {
				auto lhs = parseConcat();
				while(m_lexer.skipIf(TokenKinds::Pipe)) {
					lhs = makeNode<AlternRule>(std::move(lhs), parseConcat());
				}
				return lhs;
			}


			NodePtr parseConcat() {
				auto lhs = parseRepeat();
				while(isAtom()) {
					lhs = makeNode<ConcatRule>(std::move(lhs), parseRepeat());
				}
				return lhs;
			}


			NodePtr parseRepeat() {
				auto inner = parseAtom();
				while(true) {
					if(m_lexer.skipIf(TokenKinds::Star)) {
						inner = makeNode<StarRule>(std::move(inner));
					} else if(m_lexer.skipIf(TokenKinds::Plus)) {
						inner = makeNode<PlusRule>(std::move(inner));
					} else if(m_lexer.skipIf(TokenKinds::QuestionMark)) {
						inner = makeNode<OptionalRule>(std::move(inner));
					} else {
						break;
					}
				}
				return inner;
			}


			NodePtr parseAtom() {
				switch(m_lexer.peek().kind()) {
					case TokenKinds::Ident: {
						return makeNode<SymbolAtom>(m_lexer.lex());
					}
					case TokenKinds::LeftParen: {
						const auto openParen = m_lexer.lex();
						if(m_lexer.skipIf(TokenKinds::RightParen)) {
							return makeNode<EmptyRule>();
						}

						auto subrule = parseRule();
						if(!m_lexer.skipIf(TokenKinds::RightParen)) {
							err::misplacedChar(openParen.loc(), '(');
						}
						return subrule;
					}
					case TokenKinds::PatternString: {
						return makeNode<InlineToken>(m_lexer.lex());
					}
					case TokenKinds::RightParen: {
						err::misplacedChar(m_lexer.loc(), ')');
					}
					default: {
						const auto& tok = m_lexer.peek();
						err::misplacedToken(tok.loc(), tok.text());
					}
				}
			}


			bool isAtom() {
				switch(m_lexer.peek().kind()) {
					case TokenKinds::Ident:
					case TokenKinds::LeftParen:
					case TokenKinds::PatternString: {
						return true;
					}
					default: {
						return false;
					}
				}
			}


			template <TokenKinds tok>
			Token expect() {
				if(const auto& peekTok = m_lexer.peek(); !peekTok.is<tok>()) {
					err::unmatchedToken(peekTok.loc(), describeToken(tok), describeToken(peekTok.kind()));
				}
				return m_lexer.lex();
			}


			static std::string describeToken(TokenKinds tok) {
				switch(tok) {
					case TokenKinds::EmptyToken: return "empty token";
					case TokenKinds::Eof: return "end of file";

					case TokenKinds::Ident: return "identifier";
					case TokenKinds::PatternString: return "string pattern";

					case TokenKinds::Star: return "'*'";
					case TokenKinds::Plus: return "'+'";
					case TokenKinds::QuestionMark: return "'?'";
					case TokenKinds::Pipe: return "'|'";
					case TokenKinds::Semicolon: return "';'";
					case TokenKinds::Equals: return "'='";

					case TokenKinds::LeftBrace: return "'{'";
					case TokenKinds::RightBrace: return "'}'";

					case TokenKinds::LeftParen: return "'('";
					case TokenKinds::RightParen: return "')'";
				}
				return "";
			}


			Lexer m_lexer;
		};
	}


	NodePtr Parser::parse(std::string_view input) {
		auto in = std::istringstream(std::string(input));
		return parse(in);
	}


	NodePtr Parser::parse(std::istream& input) {
		return ParserImpl(input).run();
	}
}