#include "BnfParser.hpp"

namespace parsec {
	BnfTokenKinds BnfParser::PeekToken() {
		return lexer.Peek();
	}

	BnfToken BnfParser::GetToken() {
		return lexer.Lex();		
	}
	void BnfParser::SkipToken() {
		GetToken();
	}

	void BnfParser::MatchToken(std::string_view text, gsl::czstring msg) {
		if(PeekToken() != BnfTokenKinds::Ident) {
			throw ParseError("identifier expected", lexer.GetInputPos());
		}
		const auto tok = GetToken();
		if(tok.GetText() != text) {
			throw ParseError(msg, tok.GetLocation());
		}
	}
	void BnfParser::MatchToken(BnfTokenKinds tok, gsl::czstring msg) {
		if(PeekToken() != tok) {
			throw ParseError(msg, lexer.GetInputPos());
		}
		SkipToken();
	}

	void BnfParser::ParseTokenDef() {
		MatchToken(BnfTokenKinds::Ident, "a token must start with an identifier");
		MatchToken(BnfTokenKinds::Equals, "'=' expected");
		MatchToken(BnfTokenKinds::Regex, "string literal expected");
		MatchToken(BnfTokenKinds::Semicolon, "a token must end with ';'");
	}
	
	void BnfParser::ParseTokenList() {
		MatchToken(BnfTokenKinds::LeftBrace, "a token list must start with '{'");
		while(PeekToken() == BnfTokenKinds::Ident) {
			ParseTokenDef();
		}
		MatchToken(BnfTokenKinds::RightBrace, "expected an identifier or '}'");
	}

	void BnfParser::ParseGrammarDef() {
		MatchToken("tokens", "invalid list type");
		ParseTokenList();
	}

	void BnfParser::Parse() {
		ParseGrammarDef();
	}
}