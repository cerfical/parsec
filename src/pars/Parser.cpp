#include "pars/Parser.hpp"

#include "pars/Lexer.hpp"
#include "pars/ast.hpp"

#include "errors/UnexpectedTokenError.hpp"
#include "errors/TokenMismatchError.hpp"

#include <sstream>

namespace parsec::pars {
	namespace {
		class ParseStream {
		public:

			explicit ParseStream(std::istream& input)
				: m_lexer(input)
			{ }

			ast::NodePtr operator()() {
				return parseSpec();
			}

		private:
			static std::string describeToken(TokenKinds tok) {
				switch(tok) {
					case TokenKinds::EmptyToken: return "an empty token";
					case TokenKinds::Eof: return "an end of file";
					
					case TokenKinds::Ident: return "an identifier";
					case TokenKinds::PatternString: return "a string pattern";
					case TokenKinds::RawString: return "a string literal";

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


			[[noreturn]] void unexpectedTokenError() {
				const auto& tok = m_lexer.peek();
				throw UnexpectedTokenError(
					tok.loc(), tok.text()
				);
			}

			[[noreturn]] void unmatchedParenError(const SourceLoc& parenLoc) {
				throw ParseError(ErrorCodes::UnmatchedParenthesis, parenLoc);
			}

			[[noreturn]] void unmatchedTokenError(TokenKinds expect) {
				throw TokenMismatchError(m_lexer.loc(),
					describeToken(expect),
					describeToken(m_lexer.peek().kind())
				);
			}


			Token expectToken(TokenKinds kind) {
				if(!m_lexer.peek().is(kind)) {
					unmatchedTokenError(kind);
				}
				return m_lexer.lex();
			}

			bool isAtom() {
				switch(m_lexer.peek().kind()) {
					case TokenKinds::Ident:
					case TokenKinds::LeftParen:
					case TokenKinds::PatternString:
					case TokenKinds::RawString: {
						return true;
					}
					default: {
						return false;
					}
				}
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
							unmatchedParenError(openParen.loc());
						}
						return subrule;
					}
					case TokenKinds::PatternString: {
						return ast::makeNode<ast::InlineToken>(m_lexer.lex());
					}
					case TokenKinds::RawString: {
						return ast::makeNode<ast::InlineToken>(m_lexer.lex());
					}
					case TokenKinds::RightParen: {
						unmatchedParenError(m_lexer.loc());
					}
					default: {
						unexpectedTokenError();
					}
				}
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

			ast::NodePtr parseConcat() {
				auto lhs = parseRepeat();
				while(isAtom()) {
					lhs = ast::makeNode<ast::ConcatRule>(std::move(lhs), parseRepeat());
				}
				return lhs;
			}

			ast::NodePtr parseAltern() {
				auto lhs = parseConcat();
				while(m_lexer.skipIf(TokenKinds::Pipe)) {
					lhs = ast::makeNode<ast::AlternRule>(std::move(lhs), parseConcat());
				}
				return lhs;
			}

			ast::NodePtr parseRule() {
				return parseAltern();
			}


			ast::NodePtr parseTokenList() {
				auto tokens = ast::makeNode<ast::EmptyNode>();

				expectToken(TokenKinds::LeftBrace);
				while(!m_lexer.skipIf(TokenKinds::RightBrace)) {
					auto name = expectToken(TokenKinds::Ident);
					expectToken(TokenKinds::Equals);
					
					if(const auto kind = m_lexer.peek().kind(); kind != TokenKinds::PatternString && kind != TokenKinds::RawString) {
						unexpectedTokenError();
					}
					
					auto pattern = ast::makeNode<ast::NamedToken>(name, m_lexer.lex());
					expectToken(TokenKinds::Semicolon);
					
					tokens = ast::makeNode<ast::ListNode>(
						std::move(tokens),
						std::move(pattern)
					);
				}

				return tokens;
			}

			ast::NodePtr parseRuleList() {
				auto rules = ast::makeNode<ast::EmptyNode>();

				expectToken(TokenKinds::LeftBrace);
				while(!m_lexer.skipIf(TokenKinds::RightBrace)) {
					auto name = expectToken(TokenKinds::Ident);
					expectToken(TokenKinds::Equals);

					auto rule = ast::makeNode<ast::NamedRule>(
						std::move(name),
						parseRule()
					);
					expectToken(TokenKinds::Semicolon);

					rules = ast::makeNode<ast::ListNode>(
						std::move(rules),
						std::move(rule)
					);
				}

				return rules;
			}

			ast::NodePtr parseSpec() {
				auto spec = ast::makeNode<ast::EmptyNode>();
				while(!m_lexer.isEof()) {
					if(m_lexer.skipIf("tokens")) {
						spec = ast::makeNode<ast::ListNode>(std::move(spec), parseTokenList());
					} else if(m_lexer.skipIf("rules")) {
						spec = ast::makeNode<ast::ListNode>(std::move(spec), parseRuleList());
					} else {
						unexpectedTokenError();
					}
				}
				return spec;
			}


			Lexer m_lexer;
		};

	}


	ast::NodePtr Parser::parse(std::string_view input) {
		auto in = std::istringstream(std::string(input));
		return parse(in);
	}

	ast::NodePtr Parser::parse(std::istream& input) {
		return ParseStream(input)();
	}
}