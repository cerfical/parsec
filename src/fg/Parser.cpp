#include "fg/Parser.hpp"
#include "fg/nodes.hpp"

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


	bool Parser::ruleAtomStart(){
		switch(m_lexer.peek().kind()) {
			case Token::Ident: return true;
			default: return false;
		}
	}

	void Parser::parseRuleAtom() {
		switch(m_lexer.peek().kind()) {
			case Token::Ident: {
				m_ruleExpr = std::make_unique<RuleRef>(m_lexer.lex().text());
				break;
			}
			default: parseError("expected a rule identifier");
		}
	}

	void Parser::parseRuleConcat() {
		auto left = (parseRuleAtom(), std::move(m_ruleExpr));
		while(ruleAtomStart()) {
			auto right = (parseRuleAtom(), std::move(m_ruleExpr));
			left = std::make_unique<ConcatNode>(
				std::move(left),
				std::move(right)
			);
		}
		m_ruleExpr = std::move(left);
	}

	void Parser::parseRuleAltern() {
		auto left = (parseRuleConcat(), std::move(m_ruleExpr));
		while(m_lexer.skipIf(Token::Pipe)) {
			auto right = (parseRuleConcat(), std::move(m_ruleExpr));
			left = std::make_unique<AlternNode>(
				std::move(left),
				std::move(right)
			);
		}
		m_ruleExpr = std::move(left);
	}

	void Parser::parseRuleExpr() {
		parseRuleAltern();
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


	void Parser::parseRuleDef() {
		const auto ruleName = m_lexer.lex().text();
		
		if(!m_lexer.skipIf(Token::Equals)) {
			parseError("expected an '='");
		}

		parseRuleExpr();
		auto& rule = *m_syntaxRules.try_emplace(ruleName).first;
		if(rule.second) {
			rule.second = std::make_unique<AlternNode>(std::move(rule.second), std::move(m_ruleExpr));
		} else {
			rule.second = std::move(m_ruleExpr);
		}

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


	LangGrammar Parser::parse(std::string_view input) {
		auto str = std::istringstream(std::string(input));
		return parse(str);
	}

	LangGrammar Parser::parse(std::istream& input) {
		// make the parser state to reset after the parse is complete
		const auto sentry = gsl::finally([this]() {
			m_lexer = Lexer();
			m_tokens.clear();
		});
		m_lexer = Lexer(input);

		parseGrammar();

		LangGrammar grammar;
		for(auto& tok : m_tokens) {
			grammar.addTokenRule(tok.first, regex::RegEx(std::move(tok.second)));
		}
		for(auto& rule : m_syntaxRules) {
			grammar.addSyntaxRule(rule.first, std::move(rule.second));
		}
		return grammar;
	}
}