#include "fg/Parser.hpp"

#include "regex/Parser.hpp"
#include "utils/ParseError.hpp"
#include "fg/rules/rules.hpp"

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
			/*
				the error location needs to be adjusted as the regex::Parser operates only on a subset of the original input stream
				also the "+ 1" term is needed to account for double/single quotes delimiting the regex token
			*/
			throw ParseError(
				err.what(),
				SourceLoc(
					regex.location().startCol() + err.location().startCol() + 1,
					err.location().colCount(),
					regex.location().line(),
					regex.location().pos()
				)
			);
		}
	}

	void Parser::parseTokenDef() {
		const auto tokName = m_lexer.lex().text();
		if(m_grammar.resolveRule(tokName)) {
			parseError("duplicate rule name");
		}

		if(!m_lexer.skipIf(Token::Equals)) {
			parseError("expected an '='");
		}

		if(!m_lexer.peek().is<Token::StringLiteral>()) {
			parseError("expected a string literal");
		}

		m_grammar.addTokenRule(tokName, parseRegex(m_lexer.lex()));

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


	bool Parser::atomStart() {
		switch(m_lexer.peek().kind()) {
			case Token::Ident: return true;
			case Token::OpenParen: return true;
			default: return false;
		}
	}

	void Parser::parseAtom() {
		switch(m_lexer.peek().kind()) {
			case Token::Ident: {
				m_rule = std::make_unique<rules::Atom>(m_lexer.lex().text());
				break;
			}
			case Token::OpenParen: {
				const auto tok = m_lexer.lex();
				m_rule = (parseRule(), std::move(m_rule));
				if(!m_lexer.skipIf(Token::CloseParen)) {
					throw ParseError("unmatched parenthesis", tok.location());
				}
				break;
			}
			default: parseError("expected a rule identifier or a '('");
		}
	}

	void Parser::parseConcat() {
		auto left = (parseAtom(), std::move(m_rule));
		while(atomStart()) {
			auto right = (parseAtom(), std::move(m_rule));
			left = std::make_unique<rules::RuleConcat>(
				std::move(left),
				std::move(right)
			);
		}
		m_rule = std::move(left);
	}

	void Parser::parseAltern() {
		auto left = (parseConcat(), std::move(m_rule));
		while(m_lexer.skipIf(Token::Pipe)) {
			auto right = (parseConcat(), std::move(m_rule));
			left = std::make_unique<rules::RuleAltern>(
				std::move(left),
				std::move(right)
			);
		}
		m_rule = std::move(left);
	}

	void Parser::parseRule() {
		parseAltern();
	}


	void Parser::parseRuleDef() {
		const auto ruleName = m_lexer.lex().text();
		if(m_grammar.resolveRule(ruleName)) {
			parseError("duplicate rule name");
		}

		if(!m_lexer.skipIf(Token::Equals)) {
			parseError("expected an '='");
		}

		m_grammar.addSyntaxRule(ruleName, (parseRule(), std::move(m_rule)));

		if(!m_lexer.skipIf(Token::Semicolon)) {
			parseError("expected a ';'");
		}
	}

	void Parser::parseRuleList() {
		if(!m_lexer.skipIf(Token::OpenBrace)) {
			parseError("expected a '{'");
		}

		while(m_lexer.peek().is<Token::Ident>()) {
			parseRuleDef();
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
			} if(m_lexer.peek().text() == "rules") {
				m_lexer.skip();
				parseRuleList();
			} else {
				badTokenError();
			}
		}
	}


	Grammar Parser::parse(std::string_view input) {
		auto in = std::istringstream(std::string(input));
		return parse(in);
	}

	Grammar Parser::parse(std::istream& input) {
		m_lexer = Lexer(input);
		parseGrammar();
		return std::move(m_grammar);
	}
}