#include "pars/Parser.hpp"

#include "pars/Lexer.hpp"
#include "pars/ParseError.hpp"
#include "pars/ast.hpp"

#include <sstream>
#include <format>

namespace parsec::pars {
	namespace {

		class DescribeToken : TokenKindVisitor {
		public:

			auto operator()(TokenKinds tok) {
				visit(tok);
				return m_desc;
			}

		private:
			void onNil() override {
				m_desc = "an empty token";
			}

			void onEof() override {
				m_desc = "an end of file";
			}


			void onIdent() override {
				m_desc = "an identifier";
			}

			void onPattern() override {
				m_desc = "a string pattern";
			}


			void onStar() override {
				m_desc = "a '*'";
			}
			
			void onPlus() override {
				m_desc = "a '+'";
			}

			void onQo() override {
				m_desc = "a '?'";
			}
			
			void onPipe() override {
				m_desc = "a '|'";
			}
			

			void onSemicolon() override {
				m_desc = "a ';'";
			}
			
			void onEquals() override {
				m_desc = "an '='";
			}

			void onLeftBrace() override {
				m_desc = "a '{'";
			}

			void onRightBrace() override {
				m_desc = "a '}'";
			}
			
			void onLeftParen() override {
				m_desc = "a '('";
			}

			void onRightParen() override {
				m_desc = "a ')'";
			}


			std::string_view m_desc;
		};


		class ParseStream {
		public:

			explicit ParseStream(std::istream& input)
				: m_lexer(input)
			{ }

			ast::NodePtr operator()() {
				return parseSpec();
			}

		private:
			[[noreturn]] void unexpectedTokenError() {
				const auto& tok = m_lexer.peek();
				throw ParseError(std::format("unexpected \"{}\"",
						tok.text()
					), tok.loc()
				);
			}

			[[noreturn]] void unmatchedParenError(const SourceLoc& parenLoc) {
				throw ParseError("unmatched parenthesis", parenLoc);
			}

			[[noreturn]] void unmatchedTokenError(TokenKinds expect) {
				throw ParseError(
					std::format("expected {}, but got {}",
						DescribeToken()(expect),
						DescribeToken()(m_lexer.peek().kind())
					),
					m_lexer.loc()
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
					case TokenKinds::Pattern: {
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
						return ast::makeNode<ast::NameRule>(m_lexer.lex());
					}
					case TokenKinds::LeftParen: {
						const auto openParen = m_lexer.lex();
						if(m_lexer.skipIf(TokenKinds::RightParen)) {
							return ast::makeNode<ast::NilRule>();
						}

						auto subrule = parseRule();
						if(!m_lexer.skipIf(TokenKinds::RightParen)) {
							unmatchedParenError(openParen.loc());
						}
						return subrule;
					}
					case TokenKinds::Pattern: {
						return ast::makeNode<ast::InlinePattern>(m_lexer.lex());
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
					} else if(m_lexer.skipIf(TokenKinds::Qo)) {
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
				auto tokens = ast::makeNode<ast::NilNode>();

				expectToken(TokenKinds::LeftBrace);
				while(!m_lexer.skipIf(TokenKinds::RightBrace)) {
					auto name = expectToken(TokenKinds::Ident);
					expectToken(TokenKinds::Equals);
					
					auto pattern = ast::makeNode<ast::NamedPattern>(
						std::move(name),
						expectToken(TokenKinds::Pattern)
					);
					expectToken(TokenKinds::Semicolon);
					
					tokens = ast::makeNode<ast::NodeList>(
						std::move(pattern),
						std::move(tokens)
					);
				}

				return tokens;
			}

			ast::NodePtr parseRuleList() {
				auto rules = ast::makeNode<ast::NilNode>();

				expectToken(TokenKinds::LeftBrace);
				while(!m_lexer.skipIf(TokenKinds::RightBrace)) {
					auto name = expectToken(TokenKinds::Ident);
					expectToken(TokenKinds::Equals);

					auto rule = ast::makeNode<ast::NamedRule>(
						std::move(name),
						parseRule()
					);
					expectToken(TokenKinds::Semicolon);

					rules = ast::makeNode<ast::NodeList>(
						std::move(rule),
						std::move(rules)
					);
				}

				return rules;
			}

			ast::NodePtr parseSpec() {
				auto spec = ast::makeNode<ast::NilNode>();
				while(!m_lexer.isEof()) {
					if(m_lexer.skipIf("tokens")) {
						spec = ast::makeNode<ast::NodeList>(parseTokenList(), std::move(spec));
					} else if(m_lexer.skipIf("rules")) {
						spec = ast::makeNode<ast::NodeList>(parseRuleList(), std::move(spec));
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