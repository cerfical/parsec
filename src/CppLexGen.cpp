#include "CppLexGen.hpp"

#include <boost/tokenizer.hpp>
#include <sstream>

namespace {
	using namespace parsec;
	using namespace boost;

	using str = const char*;

	/** @{ */
	constexpr str wsPattern = "ws";

	constexpr struct {
		str startPos = "startPos";
		str colNo = "colNo";
		str colCount = "colCount";
		str lineNo = "lineNo";
		
		constexpr operator str() const noexcept {
			return "SourceLoc";
		}
	} sourceLoc;

	constexpr struct {
		str loc = "loc";

		constexpr operator str() const noexcept {
			return "LexError";
		}
	} lexError;

	constexpr struct {
		str text = "text";
		str kind = "kind";
		str loc = "loc";

		constexpr operator str() const noexcept {
			return "Token";
		}
	} token;

	constexpr struct {
		str input = "m_input";
		str buffer = "m_buffer";
		str pos = "m_pos";

		str eof = "IsEof";
		str peek = "PeekChar";
		str next = "NextChar";
		str make = "MakeToken";

		struct {
			str lex = "lex";
			str start = "start";
			str prefix = "state";
			
			struct {
				str msg = "ill-formed token"; 
				
				constexpr operator str() const noexcept {
					return "error";
				}
			} error;

			constexpr operator str() const noexcept {
				return "Lex";
			}
		} lex;

		constexpr operator str() const noexcept {
			return "Lexer";
		}
	} lexer;

	constexpr struct {
		str eof = "Eof";

		constexpr operator str() const noexcept {
			return "Tokens";
		}
	} tokenKinds;
	/** @} */

	/** @{ */
	std::string ToIdentifier(const std::string& str) {
		const static char_separator<char> delim(" \f\n\r\t\v-_");
		const auto toks = tokenizer(str, delim);

		// transform a string to a PascalCase identifier
		std::string id;
		for(const auto& tok : toks) {
			auto t = tok;
			t.front() = std::toupper(t.front());
			id += t;
		}
		return id;
	}

	std::string MakeStateLabel(int state) {
		return (std::ostringstream() << lexer.lex.prefix << state).str();
	}
	std::string MakeTokenKind(const TokenPattern& tok) {
		return ToIdentifier(tok.GetName());
	}
	/** @} */
}

namespace parsec {
	void LexCppGen::OnPreState(StateID state) {
		m_out.Up().PrintLine(MakeStateLabel(state), ':');
		m_out.PrintLine(lexer.next, "();");
		if(state == 0) {
			m_out.Up().PrintLine(lexer.lex.start, ':');
		}
	}
	
	void LexCppGen::OnPreTransitions() {
		m_out.PrintLine("if(!", lexer.eof, "()) {" );
		m_out = m_out.Down();
		m_out.PrintLine("switch(" , lexer.peek,"()) {");
	}

	void LexCppGen::OnTransition(StateID toState, char label) {
		m_out.Down().PrintLine("case '", CharUtils::EscapeChar(label), "': ",
			"goto ", MakeStateLabel(toState), ';'
		);
	}
	
	void LexCppGen::OnPostTransitions() {
		m_out.PrintLine('}');
		m_out = m_out.Up();
		m_out.PrintLine('}');
	}

	void LexCppGen::OnAccept(const TokenPattern& match) {
		if(match.GetName() != wsPattern) {
			m_out.PrintLine("return ", lexer.make, '(', tokenKinds, "::", MakeTokenKind(match), ");");
		} else {
			m_out.PrintLine("goto ", lexer.lex.lex, ';');
		}
	}

	void LexCppGen::OnReject() {
		m_out.PrintLine("goto ", lexer.lex.error, ';');
	}

	void LexCppGen::OnPostState(StateID state) {
		m_out.PrintLine();
	}


	void LexCppGen::GenHeader() {
		GenSourceLoc();
		m_out.PrintLine();
		GenTokenKinds();
		m_out.PrintLine();
		GenLexError();
		m_out.PrintLine();
		GenToken();
		m_out.PrintLine();
		GenLexer();
	}

	void LexCppGen::GenSourceLoc() {
		m_out.PrintLine("struct ", sourceLoc, " {").Down()
			.PrintLine("int ", sourceLoc.startPos, " = { };")
			.PrintLine("int ", sourceLoc.colNo, " = { };")
			.PrintLine("int ", sourceLoc.colCount, " = { };")
			.PrintLine("int ", sourceLoc.lineNo, " = { };");
		m_out.PrintLine("};");
	}

	void LexCppGen::GenLexError() {
		m_out.PrintLine("class ", lexError, " : public std::runtime_error {");
		m_out.PrintLine("public:");
		m_out.Down()
			.PrintLine(lexError, "(const char* msg, const ", sourceLoc, "& loc)")
			.PrintLine(" : runtime_error(msg), ", lexError.loc, "(loc)")
			.PrintLine("{ }\n")
			.PrintLine(lexError, "(const std::string& msg, const ", sourceLoc, "& loc)")
			.PrintLine(" : runtime_error(msg), ", lexError.loc, "(loc)")
			.PrintLine("{ }\n")
			.PrintLine(sourceLoc, ' ', lexError.loc, " = { };");
		m_out.PrintLine("};");
	}

	void LexCppGen::GenTokenKinds() {
		m_out.PrintLine("enum class ", tokenKinds, " {");
		m_out.Down().Print(tokenKinds.eof);
		for(const auto& tok : GetInputGrammar()->GetTokens()) {
			if(tok.GetName() != wsPattern) {
				m_out.RawPrint(",\n").Down()
					.Print(MakeTokenKind(tok));
			}
		}
		m_out.PrintLine();
		m_out.PrintLine("};");
	}

	void LexCppGen::GenToken() {
		m_out.PrintLine("struct ", token, " {").Down()
			.PrintLine("std::string ", token.text, ';')
			.PrintLine(tokenKinds, ' ', token.kind, " = ", tokenKinds, "::", tokenKinds.eof, ';')
			.PrintLine(sourceLoc, ' ', token.loc, " = { };");
		m_out.PrintLine("};");
	}

	void LexCppGen::GenLexer() {
		m_out.PrintLine("class ", lexer, " {");
		m_out = m_out.Down();
		
		m_out.Up().PrintLine("public:");
		GenLexerInterface();
		
		m_out.Up().PrintLine("private:");
		GenLexerImpl();

		m_out = m_out.Up();
		m_out.PrintLine("};");
	}


	void LexCppGen::GenLexerInterface() {
		m_out.PrintLine(lexer, "() = default;");
		m_out.PrintLine(lexer, "(std::istream* input) noexcept");
		m_out.PrintLine(" : ", lexer.input, "(input)");
		m_out.PrintLine("{ }");

		m_out.PrintLine();
		
		m_out.PrintLine(lexer, '(', lexer, "&&) = default;");
		m_out.PrintLine(lexer, "& operator=(", lexer, "&&) = default;");

		m_out.PrintLine();
		
		m_out.PrintLine(lexer, "(const ", lexer, "&) = delete;");
		m_out.PrintLine(lexer, "& operator=(const ", lexer, "&) = delete;");

		m_out.PrintLine();
		GenLexerLex();
		m_out.PrintLine();
	}

	void LexCppGen::GenLexerLex() {
		m_out.PrintLine(token, ' ', lexer.lex, "() {");
		m_out.PrintLine(lexer.lex.lex, ':');
		m_out = m_out.Down();
		m_out.PrintLine(lexer.buffer, ".clear();");
		m_out.PrintLine(lexer.pos, '.', sourceLoc.colNo, " += ", lexer.pos, '.', sourceLoc.colCount, ';');
		m_out.PrintLine(lexer.pos, '.', sourceLoc.colCount, " = 0;");
		m_out.PrintLine("if(!", lexer.eof, "()) {" );
		m_out.Down().PrintLine("goto ", lexer.lex.start, ';');
		m_out.PrintLine('}');
		m_out.PrintLine("return ", lexer.make, '(', tokenKinds, "::", tokenKinds.eof, ");\n");
		
		LexGen::Gen();

		m_out.Up().PrintLine(lexer.lex.error, ':');
		m_out.PrintLine("throw ", lexError, "(\"", lexer.lex.error.msg, "\", ", lexer.pos, ");");
		m_out = m_out.Up();
		m_out.PrintLine('}');
	}


	void LexCppGen::GenLexerImpl() {
		GenLexerEof();
		GenLexerPeek();
		m_out.PrintLine();

		GenLexerNext();
		m_out.PrintLine();
	
		GenLexerMake();
		m_out.PrintLine();

		m_out.PrintLine("std::istream* ", lexer.input, " = nullptr;");
		m_out.PrintLine("std::string ", lexer.buffer, ';');
		m_out.PrintLine(sourceLoc, ' ', lexer.pos, " = { };");
	}

	void LexCppGen::GenLexerEof() {
		m_out.PrintLine("bool ", lexer.eof, "() const {").Down()
			.PrintLine("if(", lexer.input, ") {").Down()
				.PrintLine("if(", lexer.input, "->peek() != std::char_traits<char>::eof()) {").Down()
					.PrintLine("return false;").Up()
				.PrintLine('}')
				.PrintLine(lexer.input, "->clear(", lexer.input, "->rdstate() ^ std::ios::eofbit);").Up()
			.PrintLine('}')
			.PrintLine("return true;");
		m_out.PrintLine('}');
	}

	void LexCppGen::GenLexerPeek() {
		m_out.PrintLine("char ", lexer.peek, "() const {").Down()
			.PrintLine("return static_cast<char>(", lexer.input, "->peek());");
		m_out.PrintLine('}');
	}

	void LexCppGen::GenLexerNext() {
		m_out.PrintLine("void ", lexer.next, "() {").Down()
			.PrintLine(lexer.pos, '.', sourceLoc.colCount,"++;")
			.PrintLine("if(", lexer.peek, "() == '\\n') {").Down()
				.PrintLine(lexer.pos, '.', sourceLoc.startPos, " += ", lexer.pos, '.', sourceLoc.colNo, " + ", lexer.pos, '.', sourceLoc.colCount, ';')
				.PrintLine(lexer.pos, '.', sourceLoc.colNo, " = ", lexer.pos, '.', sourceLoc.colCount, " = 0;")
				.PrintLine(lexer.pos, '.', sourceLoc.lineNo, "++;").Up()
			.PrintLine('}')
			.PrintLine(lexer.buffer, " += ", lexer.input, "->get();");
		m_out.PrintLine('}');
	}

	void LexCppGen::GenLexerMake() {
		m_out.PrintLine(token, ' ', lexer.make, '(', tokenKinds, " kind) const {").Down()
			.PrintLine("return ", token, '(', lexer.buffer, ", kind, ", lexer.pos, ");");
		m_out.PrintLine('}');
	}


	void LexCppGen::Gen() {
		GenHeader();
	}
}