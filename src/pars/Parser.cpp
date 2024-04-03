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


			ast::NodePtr run() {
				return parseSpec();
			}


		private:

			ast::NodePtr parseSpec() {
				auto spec = ast::makeNode<ast::EmptyNode>();
				while(!m_lexer.isEof()) {
					if(m_lexer.skipIf("tokens")) {
						spec = ast::makeNode<ast::ListNode>(std::move(spec), parseTokenList());
					} else if(m_lexer.skipIf("rules")) {
						spec = ast::makeNode<ast::ListNode>(std::move(spec), parseRuleList());
					} else {
						const auto& tok = m_lexer.peek();
						err::misplacedToken(tok.loc(), tok.text());
					}
				}
				return spec;
			}


			ast::NodePtr parseTokenList() {
				auto tokens = ast::makeNode<ast::EmptyNode>();
				expect<TokenKinds::LeftBrace>();

				while(!m_lexer.skipIf(TokenKinds::RightBrace)) {
					const auto name = expect<TokenKinds::Ident>();
					expect<TokenKinds::Equals>();

					auto pattern = ast::makeNode<ast::NamedToken>(name, expect<TokenKinds::PatternString>());
					expect<TokenKinds::Semicolon>();

					tokens = ast::makeNode<ast::ListNode>(
						std::move(tokens),
						std::move(pattern)
					);
				}

				return tokens;
			}


			ast::NodePtr parseRuleList() {
				auto rules = ast::makeNode<ast::EmptyNode>();
				expect<TokenKinds::LeftBrace>();

				while(!m_lexer.skipIf(TokenKinds::RightBrace)) {
					const auto name = expect<TokenKinds::Ident>();
					expect<TokenKinds::Equals>();

					auto rule = ast::makeNode<ast::NamedRule>(
						name,
						parseRule()
					);
					expect<TokenKinds::Semicolon>();

					rules = ast::makeNode<ast::ListNode>(
						std::move(rules),
						std::move(rule)
					);
				}

				return rules;
			}


			ast::NodePtr parseRule() {
				return parseAltern();
			}


			ast::NodePtr parseAltern() {
				auto lhs = parseConcat();
				while(m_lexer.skipIf(TokenKinds::Pipe)) {
					lhs = ast::makeNode<ast::AlternRule>(std::move(lhs), parseConcat());
				}
				return lhs;
			}


			ast::NodePtr parseConcat() {
				auto lhs = parseRepeat();
				while(isAtom()) {
					lhs = ast::makeNode<ast::ConcatRule>(std::move(lhs), parseRepeat());
				}
				return lhs;
			}


			ast::NodePtr parseRepeat() {
				auto inner = parseAtom();
				while(true) {
					if(m_lexer.skipIf(TokenKinds::Star)) {
						inner = ast::makeNode<ast::StarRule>(std::move(inner));
					} else if(m_lexer.skipIf(TokenKinds::Plus)) {
						inner = ast::makeNode<ast::PlusRule>(std::move(inner));
					} else if(m_lexer.skipIf(TokenKinds::QuestionMark)) {
						inner = ast::makeNode<ast::OptionalRule>(std::move(inner));
					} else {
						break;
					}
				}
				return inner;
			}


			ast::NodePtr parseAtom() {
				switch(m_lexer.peek().kind()) {
					case TokenKinds::Ident: {
						return ast::makeNode<ast::SymbolRule>(m_lexer.lex());
					}
					case TokenKinds::LeftParen: {
						const auto openParen = m_lexer.lex();
						if(m_lexer.skipIf(TokenKinds::RightParen)) {
							return ast::makeNode<ast::EmptyRule>();
						}

						auto subrule = parseRule();
						if(!m_lexer.skipIf(TokenKinds::RightParen)) {
							err::misplacedChar(openParen.loc(), '(');
						}
						return subrule;
					}
					case TokenKinds::PatternString: {
						return ast::makeNode<ast::InlineToken>(m_lexer.lex());
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
					case TokenKinds::EmptyToken: return "an empty token";
					case TokenKinds::Eof: return "an end of file";

					case TokenKinds::Ident: return "an identifier";
					case TokenKinds::PatternString: return "a string pattern";

					case TokenKinds::Star: return "a '*'";
					case TokenKinds::Plus: return "a '+'";
					case TokenKinds::QuestionMark: return "a '?'";
					case TokenKinds::Pipe: return "a '|'";
					case TokenKinds::Semicolon: return "a ';'";
					case TokenKinds::Equals: return "an '='";

					case TokenKinds::LeftBrace: return "a '{'";
					case TokenKinds::RightBrace: return "a '}'";

					case TokenKinds::LeftParen: return "a '('";
					case TokenKinds::RightParen: return "a ')'";
				}
				return "";
			}


			Lexer m_lexer;
		};
	}


	ast::NodePtr Parser::parse(std::string_view input) {
		auto in = std::istringstream(std::string(input));
		return parse(in);
	}


	ast::NodePtr Parser::parse(std::istream& input) {
		return ParserImpl(input).run();
	}
}