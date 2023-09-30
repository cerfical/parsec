#include "fg/Parser.hpp"

#include "regex/Parser.hpp"
#include "regex/nodes.hpp"

#include <sstream>

namespace parsec::fg {
	void Parser::parseError(gsl::czstring msg) {
		throw ParseError(msg, m_lexer.peek().location());
	}

	void Parser::badTokenError() {
		const auto msg = (std::ostringstream()
			<< "unexpected \"" << m_lexer.peek().text() << '\"'
		).str();
		parseError(msg.c_str());
	}


	std::unique_ptr<regex::ExprNode> Parser::parseRegex(const Token& regex) {
		try {
			return regex::Parser().parse(regex.text());
		} catch(const ParseError& err) {
			// make in-regex location relative to the input
			throw ParseError(err.what(), SourceLoc(
				regex.location().startCol() + err.location().startCol() + 1,
				err.location().colCount(),
				regex.location().line(), regex.location().pos()
			));
		}
	}


	void Parser::parseTokenDef() {
		// allocate space in the table for a new token
		auto& tok = *m_tokens.try_emplace(m_lexer.lex().text()).first;
		
		if(!m_lexer.skipIf(Token::Equals)) {
			parseError("expected an '='");
		}
		if(!m_lexer.peek().is<Token::StringLiteral>()) {
			parseError("expected a string literal");
		}

		// add a new pattern to the token
		auto regex = parseRegex(m_lexer.lex());
		if(tok.second) {
			tok.second = std::make_unique<regex::AlternExpr>(std::move(tok.second), std::move(regex));
		} else {
			tok.second = std::move(regex);
		}

		if(!m_lexer.skipIf(Token::Semicolon)) {
			parseError("expected a ';'");
		}
	}

	void Parser::parseTokenList() {
		if(!m_lexer.skipIf(Token::OpenBrace)) {
			parseError("expected a '{'");
		}

		while(m_lexer.peek().is<Token::Ident>()) {
			parseTokenDef();
		}

		if(!m_lexer.skipIf(Token::CloseBrace)) {
			parseError("expected a '}'");
		}
	}

	void Parser::parseGrammar() {
		while(!m_lexer.peek().eof()) {
			if(m_lexer.peek().text() == "tokens") {
				m_lexer.skip();
				parseTokenList();
			} else {
				badTokenError();
			}
		}
	}


	LangGrammar Parser::parse(std::string_view regex) {
		auto input = std::istringstream(std::string(regex));
		return parse(input);
	}

	LangGrammar Parser::parse(std::istream& input) {
		// ensure the parser state is reset after the parse is complete
		const auto sentry = gsl::finally([this]() {
			m_lexer = Lexer();
			m_tokens.clear();
		});
		m_lexer = Lexer(input);

		parseGrammar();

		// initialize the grammar with the parsed token definitions
		LangGrammar grammar;
		for(auto& tok : m_tokens) {
			grammar.addTokenRule(tok.first, regex::RegEx(std::move(tok.second)));
		}
		return grammar;
	}
}