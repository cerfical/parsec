#include "LexCppGenerator.hpp"
#include <boost/tokenizer.hpp>

namespace {
	using namespace parsec;

	/** @{ */
	std::string ToIdentifier(const std::string& str) {
		const static boost::char_separator<char> delim(" \f\n\r\t\v-_");
		const auto toks = boost::tokenizer<boost::char_separator<char>>(str, delim);

		// transform a string to a CamelCase identifier
		std::string id;
		for(const auto& tok : toks) {
			auto t = tok;
			t.front() = std::toupper(t.front());
			id += t;
		}
		return id;
	}

	std::string MakeStateLabel(LexState state) {
		return (std::ostringstream() << "state" << state).str();
	}

	std::string MakeTokenName(const LexPattern& pattern) {
		return ToIdentifier(pattern.GetName());
	}
	/** @} */

	/** @{ */
	constexpr auto wsPattern = "ws";

	constexpr auto locationClass = "SourceLocation";
	constexpr auto tokensEnum = "Tokens";
	constexpr auto tokenClass = "Token";
	constexpr auto lexerClass = "Lexer";
	constexpr auto lexErrorClass = "Error";

	constexpr auto eofToken = "Eof";

	constexpr auto lexFunc = "Lex";
	constexpr auto makeTokenFunc = "MakeToken";
	constexpr auto peekCharFunc = "PeekChar";
	constexpr auto nextCharFunc = "NextChar";
	constexpr auto eofFunc = "IsEndOfFile";

	constexpr auto lexLabel = "lex";
	constexpr auto startLabel = "start";
	constexpr auto errorLabel = "error";
	/** @} */
}

namespace parsec {
	void LexCppGenerator::OnPreState(LexState state) {
		out.Up().PrintLine(MakeStateLabel(state), ':');
		out.PrintLine(nextCharFunc, "();");
		if(state == 0) {
			out.Up().PrintLine(startLabel, ':');
		}
	}
	
	void LexCppGenerator::OnPreTransitions() {
		out.PrintLine("if(!", eofFunc, "()) {" );
		out.Down().PrintLine("switch(" , peekCharFunc,"()) {");
	}

	void LexCppGenerator::OnTransition(LexState toState, char label) {
		out.Down(2).PrintLine("case '", CharUtils::EscapeChar(label), "': ",
			"goto ", MakeStateLabel(toState), ';'
		);
	}
	void LexCppGenerator::OnPostTransitions() {
		out.Down().PrintLine('}');
		out.PrintLine('}');
	}

	void LexCppGenerator::OnAccept(const LexPattern& match) {
		if(match.GetName() != wsPattern) {
			out.PrintLine("return ", makeTokenFunc, '(', tokensEnum, "::", MakeTokenName(match), ");");
		} else {
			out.PrintLine("goto ", lexLabel, ';');
		}
	}

	void LexCppGenerator::OnReject() {
		out.PrintLine("goto ", errorLabel, ';');
	}

	void LexCppGenerator::OnPostState(LexState state) {
		out.PrintLine();
	}


	void LexCppGenerator::Header() {
		LocationClass();
		out.PrintLine();
		TokensEnum();
		out.PrintLine();
		TokenClass();
		out.PrintLine();
		LexerClass();
	}

	void LexCppGenerator::LocationClass() {
		out.PrintLine("struct ", locationClass, " {").Down()
			.PrintLine("int startPos = { };")
			.PrintLine("int colNo = { };")
			.PrintLine("int colCount = { };")
			.PrintLine("int lineNo = { };");
		out.PrintLine("};");
	}

	void LexCppGenerator::TokensEnum() {
		out.PrintLine("enum class ", tokensEnum, " {");
		out.Down().Print(eofToken);
		for(const auto& pattern : *GetPatterns()) {
			if(pattern.GetName() != wsPattern) {
				out.RawPrint(",\n").Down()
					.Print(MakeTokenName(pattern));
			}
		}
		out.PrintLine();
		out.PrintLine("};");
	}

	void LexCppGenerator::TokenClass() {
		out.PrintLine("struct ", tokenClass, " {").Down()
			.PrintLine("std::string text;")
			.PrintLine(tokensEnum, " kind = ", tokensEnum, "::", eofToken, ';')
			.PrintLine(locationClass, " loc = { };");
		out.PrintLine("};");
	}

	void LexCppGenerator::LexErrorClass() {
		out.PrintLine("class ", lexErrorClass, " : public std::runtime_error {");
		out.PrintLine("public:");
		out.Down()
			.PrintLine(lexErrorClass, "(const char* msg, const ", locationClass, "& loc) noexcept")
			.PrintLine(" : runtime_error(msg), loc(loc)")
			.PrintLine("{ }\n")
			.PrintLine(lexErrorClass, "(const std::string& msg, const ", locationClass, "& loc) noexcept")
			.PrintLine(" : runtime_error(msg), loc(loc)")
			.PrintLine("{ }\n")
			.PrintLine(locationClass, " loc;");
		out.PrintLine("};");
	}

	void LexCppGenerator::LexerClass() {
		out.PrintLine("class ", lexerClass, " {");
		LexerPublicSection();
		LexerPrivateSection();
		out.PrintLine("};");
	}


	void LexCppGenerator::LexerPublicSection() {
		out.PrintLine("public:");
		out = out.Down();
		LexErrorClass();
		out.PrintLine();
		LexerPublicMethods();
		out = out.Up();
	}

	void LexCppGenerator::LexerPublicMethods() {
		LexerCtors();
		LexerDtor();
		out.PrintLine();
		LexerMoveOps();
		out.PrintLine();
		LexerCopyOps();
		out.PrintLine();
		LexFunc();
		out.PrintLine();
	}

	void LexCppGenerator::LexerCtors() {
		out.PrintLine(lexerClass, "() = default;");
		out.PrintLine(lexerClass, "(std::istream* input) noexcept");
		out.PrintLine(" : input(input)");
		out.PrintLine("{ }");
	}

	void LexCppGenerator::LexerDtor() {
		out.PrintLine('~', lexerClass, "() = default;");
	}

	void LexCppGenerator::LexerMoveOps() {
		out.PrintLine(lexerClass, '(', lexerClass, "&&) = default;")
			.PrintLine(lexerClass, "& operator=(", lexerClass, "&&) = default;");
	}

	void LexCppGenerator::LexerCopyOps() {
		out.PrintLine(lexerClass, "(const ", lexerClass, "&) = delete;")
			.PrintLine(lexerClass, "& operator=(const ", lexerClass, "&) = delete;");
	}

	void LexCppGenerator::LexFunc() {
		out.PrintLine(tokenClass, ' ', lexFunc, "() {");
		out.PrintLine(lexLabel, ':');
		out = out.Down();
		out.PrintLine("buffer.clear();");
		out.PrintLine("loc.colNo += loc.colCount;");
		out.PrintLine("loc.colCount = 0;");
		out.PrintLine("if(!", eofFunc, "()) {" );
		out.Down().PrintLine("goto ", startLabel, ';');
		out.PrintLine('}');
		out.PrintLine("return ", makeTokenFunc, '(', tokensEnum, "::", eofToken, ");\n");
		
		LexGenerator::Generate();

		out.Up().PrintLine(errorLabel, ": ");
		out.PrintLine("throw ", lexErrorClass, "(\"ill-formed token\", loc)", ";");
		out = out.Up();
		out.PrintLine('}');
	}


	void LexCppGenerator::LexerPrivateSection() {
		out.PrintLine("private:");
		out = out.Down();
		LexerPrivateMethods();
		LexerPrivateVars();
		out = out.Up();
	}

	void LexCppGenerator::LexerPrivateMethods() {
		EofFunc();
		PeekCharFunc();
		out.PrintLine();
		NextCharFunc();
		out.PrintLine();
		MakeTokenFunc();
		out.PrintLine();
	}

	void LexCppGenerator::EofFunc() {
		out.PrintLine("bool ", eofFunc, "() const {").Down()
			.PrintLine("if(!input) {").Down()
				.PrintLine("return true;").Up()
			.PrintLine('}')
			.PrintLine("if(input->peek() == std::char_traits<char>::eof()) {").Down()
				.PrintLine("input->clear(input->rdstate() ^ std::ios::eofbit);")
				.PrintLine("return true;").Up()
			.PrintLine('}')
			.PrintLine("return false;");
		out.PrintLine('}');
	}

	void LexCppGenerator::PeekCharFunc() {
		out.PrintLine("char ", peekCharFunc, "() const {").Down()
			.PrintLine("return static_cast<char>(input->peek());");
		out.PrintLine('}');
	}

	void LexCppGenerator::NextCharFunc() {
		out.PrintLine("void ", nextCharFunc, "() {").Down()
			.PrintLine("loc.colCount++;")
			.PrintLine("if(", peekCharFunc, "() == '\\n') {").Down()
				.PrintLine("loc.startPos = loc.colNo + loc.colCount;")
				.PrintLine("loc.colNo = loc.colCount = 0;")
				.PrintLine("loc.lineNo++;").Up()
			.PrintLine('}')
			.PrintLine("buffer += input->get();");
		out.PrintLine('}');
	}

	void LexCppGenerator::MakeTokenFunc() {
		out.PrintLine(tokenClass, ' ', makeTokenFunc, '(', tokensEnum," kind) {").Down()
			.PrintLine("return ", tokenClass, "(buffer, kind, loc);");
		out.PrintLine('}');
	}

	void LexCppGenerator::LexerPrivateVars() {
		out.PrintLine("std::istream* input = nullptr;");
		out.PrintLine("std::string buffer;");
		out.PrintLine(locationClass, " loc = { };");
	}


	void LexCppGenerator::Generate() {
		Header();
	}
}