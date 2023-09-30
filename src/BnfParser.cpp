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

	BnfToken BnfParser::MatchToken(std::string_view text, gsl::czstring msg) {
		if(PeekToken() != BnfTokenKinds::Ident) {
			throw ParseError("identifier expected", lexer.GetInputPos());
		}

		auto tok = GetToken();
		if(tok.GetText() != text) {
			throw ParseError(msg, tok.GetLocation());
		}
		return tok;
	}
	BnfToken BnfParser::MatchToken(BnfTokenKinds tok, gsl::czstring msg) {
		if(PeekToken() != tok) {
			throw ParseError(msg, lexer.GetInputPos());
		}
		return GetToken();
	}

	void BnfParser::ParseToken() {
		const auto tokName = MatchToken(BnfTokenKinds::Ident, "a token must start with an identifier");
		MatchToken(BnfTokenKinds::Equals, "'=' expected");
		
		const auto tokRegex = MatchToken(BnfTokenKinds::Regex, "string literal expected");
		MatchToken(BnfTokenKinds::Semicolon, "a token must end with ';'");
		try {
			grammar.AddToken(tokName.GetText(), tokRegex.GetText());
		} catch(const ParseError& err) {
			throw ParseError("malformed regular expression", tokRegex.GetLocation());
		}
	}
	void BnfParser::ParseTokenList() {
		MatchToken(BnfTokenKinds::OpenBrace, "a token list must start with '{'");
		while(PeekToken() == BnfTokenKinds::Ident) {
			ParseToken();
		}
		MatchToken(BnfTokenKinds::CloseBrace, "expected an identifier or '}'");
	}

	void BnfParser::ParseGrammar() {
		MatchToken("tokens", "invalid list type");
		ParseTokenList();
	}

	BnfGrammar BnfParser::Parse() {
		if(lexer.Peek() != BnfTokenKinds::Eof) {
			ParseGrammar();
		}
		return std::move(grammar);
	}
}