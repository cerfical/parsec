#include "BnfParser.hpp"

#include "RegExParser.hpp"
#include <sstream>

namespace parsec {
	void BnfParser::UnexpectedToken() {
		const auto& tok = m_lexer.Peek();
		const auto msg = (std::ostringstream()
			<< "unexpected \"" << tok.GetText() << '\"'
		).str();
		throw ParseError(msg, tok.GetLocation());
	}


	BnfTokenKinds BnfParser::PeekToken() {
		return m_lexer.Peek().GetKind();
	}
	BnfToken BnfParser::GetToken() {
		return m_lexer.Lex();		
	}

	bool BnfParser::SkipKeywordIf(std::string_view keyword) {
		if(m_lexer.IsKeyword(keyword)) {
			SkipToken();
			return true;
		}
		return false;
	}	
	void BnfParser::SkipToken() {
		GetToken();
	}


	BnfToken BnfParser::MatchToken(BnfTokenKinds tok, gsl::czstring msg) {
		if(PeekToken() != tok) {
			throw ParseError(msg, m_lexer.Peek().GetLocation());
		}
		return GetToken();
	}

	std::unique_ptr<RegExNode> BnfParser::ParseRegex(const BnfToken& regex) {
		try {
			return RegExParser().Parse(regex.GetText());
		} catch(const ParseError& err) {
			// make in-regex location relative to the input
			auto loc = regex.GetLocation();
			loc.colNo += err.GetLocation().colNo + 1;
			loc.colCount = err.GetLocation().colCount;

			throw ParseError(err.what(), loc);
		}
	}


	void BnfParser::ParseToken() {
		const auto tokName = MatchToken(BnfTokenKinds::Ident, "identifier expected").GetText();
		MatchToken(BnfTokenKinds::Equals, "'=' expected");
		
		const auto regexTok = MatchToken(BnfTokenKinds::Regex, "regular expression expected");
		MatchToken(BnfTokenKinds::Semicolon, "missing ';'");

		if(auto regex = ParseRegex(regexTok); auto oldRegex = std::exchange(m_tokens[tokName], std::move(regex))) {
			m_tokens[tokName] = std::make_unique<RegExAltern>(std::move(oldRegex), std::move(m_tokens[tokName]));
		}
	}

	void BnfParser::ParseTokenList() {
		MatchToken(BnfTokenKinds::OpenBrace, "'{' expected");
		while(PeekToken() == BnfTokenKinds::Ident) {
			ParseToken();
		}
		MatchToken(BnfTokenKinds::CloseBrace, "'}' or a token definition expected");
	}

	void BnfParser::ParseGrammar() {
		while(PeekToken() != BnfTokenKinds::Eof) {
			if(SkipKeywordIf("tokens")) {
				ParseTokenList();
			} else {
				UnexpectedToken();
			}
		}
	}


	void BnfParser::PopulateGrammar(Grammar& grammar) {
		for(auto& tok : m_tokens) {
			grammar.AddToken(std::move(tok.first), std::move(tok.second), gsl::narrow_cast<int>(grammar.GetTokenCount()));
		}
	}


	Grammar BnfParser::Parse() {
		Grammar grammar;
		ParseGrammar();
		PopulateGrammar(grammar);
		return grammar;
	}
}