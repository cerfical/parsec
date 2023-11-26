#include "Lexer.hpp"
#include "Error.hpp"

#include "regex/Parser.hpp"
#include "utils/utils.hpp"

#include <format>

namespace parsec {
	void Lexer::unmatched(std::string_view expected) const {
		throw Error(
			std::format("expected {}, but found {}",
				expected,
				Token::describe(
					peek().kind()
				)
			),
			loc()
		);
	}


	bool Lexer::isPatternStart() const {
		if(scanner()->isEof()) {
			return false;
		}

		const auto ch = scanner()->peek();
		if(ch == '\'' || ch == '\"') {
			return true;
		}
		return false;
	}

	bool Lexer::isIdentStart() const {
		// identifiers must start with a letter
		return !scanner()->isEof() && (utils::Chars::isAlpha(scanner()->peek()));
	}

	bool Lexer::isWordStart() const {
		if(scanner()->isEof()) {
			return false;
		}

		// words inside the identifier are defined by either the presence of some delimeter
		const auto ch = scanner()->peek();
		if((ch == '-' || ch == '_')) {
			scanner()->skip();
			return true;
		}

		// or a letter
		if(utils::Chars::isAlpha(ch)) {
			return true;
		}
		return false;
	}


	void Lexer::skipWhitespace() const {
		while(true) {
			// nothing to skip, the input is empty
			if(scanner()->isEof()) {
				break;
			}

			if(utils::Chars::isSpace(scanner()->peek())) { // space characters
				scanner()->skip();
			} else if(scanner()->skipIf("//")) { // single-line comments
				while(!scanner()->isEof() && scanner()->get() != '\n') {
					// skip until the end of line or file
				}
			} else if(scanner()->skipIf("/*")) { // multi-line comments
				while(!scanner()->skipIf("*/")) {
					// skip all characters until the end of comment token is encountered
					scanner()->skip();
				}
			} else {
				break;
			}
		}
	}
	

	TokenKinds Lexer::parseIdent() const {
		std::string upper, lower;
		while(isWordStart()) {
			// read uppercase part of a word
			while(!scanner()->isEof()) {
				const auto ch = scanner()->peek();
				if(!utils::Chars::isUpper(ch) && !utils::Chars::isDigit(ch)) {
					break;
				}
				upper += scanner()->get();
			}

			// read lowercase part of a word
			while(!scanner()->isEof()) {
				const auto ch = scanner()->peek();
				if(!utils::Chars::isLower(ch) && !utils::Chars::isDigit(ch)) {
					break;
				}
				lower += scanner()->get();
			}

			// combine the upper and lower case parts to create a valid capitalized word
			if(lower.empty()) {
				if(upper.empty()) {
					error();
				}
				consume(utils::Strings::capitalize(upper));
			} else if(upper.empty()) {
				consume(utils::Strings::capitalize(lower));
			} else {
				consume(upper + lower);
			}

			upper.clear();
			lower.clear();
		}
		return TokenKinds::Ident;
	}

	TokenKinds Lexer::parsePattern() const {
		const auto delim = scanner()->get();
		while(true) {
			if(scanner()->peek() == '\n') {
				error();
			}
			if(scanner()->skipIf(delim)) {
				break;
			}
			
			// automatically escape regex metachars in raw patterns
			const auto ch = scanner()->get();
			if(delim == '\'' && regex::Parser::isMetachar(ch)) {
				consume('\\');
			}
			consume(ch);

			// handle escaped characters
			if(ch == '\\') {
				consume(scanner()->get());
			}
		}
		return TokenKinds::Pattern;
	}

	TokenKinds Lexer::parseOperator() const {
		TokenKinds kind = {};
		switch(scanner()->peek()) {
			case '{': kind = TokenKinds::LeftBrace; break;
			case '}': kind = TokenKinds::RightBrace; break;
			case '(': kind = TokenKinds::LeftParen; break;
			case ')': kind = TokenKinds::RightParen; break;
			case ';': kind = TokenKinds::Semicolon; break;
			case '=': kind = TokenKinds::Equals; break;
			case '|': kind = TokenKinds::Pipe; break;
			case '*': kind = TokenKinds::Star; break;
			case '+': kind = TokenKinds::Plus; break;
			case '?': kind = TokenKinds::Qo; break;
			default: error();
		}
		consume(scanner()->get());
		return kind;
	}
	

	TokenKinds Lexer::onTokenParse() const {
		// remove all whitespace from the input before parsing the token
		skipWhitespace();
		reset();

		// end of file?
		if(scanner()->isEof()) {
			return TokenKinds::Eof;
		}
		
		// identifier?
		if(isIdentStart()) {
			return parseIdent();
		}
		
		// string pattern?
		if(isPatternStart()) {
			return parsePattern();
		}
		return parseOperator();
	}
}