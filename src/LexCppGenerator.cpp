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
	constexpr auto lexerClassName = "Lexer";
	constexpr auto tokensEnumName = "Tokens";
	constexpr auto eofTokenName = "Eof";

	constexpr auto lexFuncName = "Lex";
	constexpr auto peekFuncName = "PeekChar";
	constexpr auto skipFuncName = "SkipChar";
	constexpr auto eofFuncName = "IsEndOfFile";

	constexpr auto startLabelName = "start";
	constexpr auto errorLabelName = "error";
	/** @} */
}

namespace parsec {
	void LexCppGenerator::OnPreState(LexState state) {
		out.Up().PrintLine(MakeStateLabel(state), ':');
		out.PrintLine(skipFuncName, "();");
		if(state == 0) {
			out.Up().PrintLine(startLabelName, ':');
		}
	}
	
	void LexCppGenerator::OnPreTransitions() {
		out.PrintLine("if(!", eofFuncName, "()) {" );
		out.Down().PrintLine("switch(" , peekFuncName,"()) {");
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
		out.PrintLine("return ", tokensEnumName, "::", MakeTokenName(match), ';');
	}

	void LexCppGenerator::OnReject() {
		out.PrintLine("goto ", errorLabelName, ';');
	}

	void LexCppGenerator::OnPostState(LexState state) {
		out.PrintLine();
	}


	void LexCppGenerator::Header() {
		TokensEnum();
		LexerClass();
	}

	void LexCppGenerator::TokensEnum() {
		out.PrintLine("enum class ", tokensEnumName, " {");
		out.Down().Print(eofTokenName);
		for(const auto& pattern : *GetPatterns()) {
			out.RawPrint(",\n");
			out.Down().Print(MakeTokenName(pattern));
		}
		out.PrintLine();
		out.PrintLine("};\n");
	}

	void LexCppGenerator::LexerClass() {
		out.PrintLine("class ", lexerClassName, " {");
		LexerPublicSection();
		LexerPrivateSection();
		out.PrintLine("};");
	}


	void LexCppGenerator::LexerPublicSection() {
		out.PrintLine("public:");
		out = out.Down();
		LexerPublicMethods();
		out = out.Up();
	}

	void LexCppGenerator::LexerPublicMethods() {
		LexerCtor();
		LexerDtor();
		out.PrintLine();
		LexerMoveOps();
		out.PrintLine();
		LexerCopyOps();
		out.PrintLine();
		LexFunc();
		out.PrintLine();
	}

	void LexCppGenerator::LexerCtor() {
		out.PrintLine(lexerClassName, "() = default;");
		out.PrintLine(lexerClassName, "(std::istream* input) noexcept")
			.PrintLine(" : input(input)")
			.PrintLine("{ }");
	}

	void LexCppGenerator::LexerDtor() {
		out.PrintLine('~', lexerClassName, "() = default;");
	}

	void LexCppGenerator::LexerMoveOps() {
		out.PrintLine(lexerClassName, '(', lexerClassName, "&&) = default;")
			.PrintLine(lexerClassName, "& operator=(", lexerClassName, "&&) = default;");
	}

	void LexCppGenerator::LexerCopyOps() {
		out.PrintLine(lexerClassName, "(const ", lexerClassName, "&) = delete;")
			.PrintLine(lexerClassName, "& operator=(const ", lexerClassName, "&) = delete;");
	}

	void LexCppGenerator::LexFunc() {
		out.PrintLine(tokensEnumName, ' ', lexFuncName, "() {");
		out = out.Down();
		out.PrintLine("if(!", eofFuncName, "()) {" );
		out.Down().PrintLine("goto ", startLabelName, ';');
		out.PrintLine('}');
		out.PrintLine("return ", tokensEnumName, "::", eofTokenName, ";\n");
		
		LexGenerator::Generate();

		out.Up().PrintLine(errorLabelName, ": ");
		out.PrintLine("throw \"lex error\";");
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
		PeekFunc();
		out.PrintLine();
		SkipFunc();
		out.PrintLine();
	}

	void LexCppGenerator::EofFunc() {
		out.PrintLine("bool ", eofFuncName, "() const {").Down()
			.PrintLine("if(!input) {").Down()
				.PrintLine("return true;").Up()
			.PrintLine('}')
			.PrintLine("if(input->peek() == EOF) {").Down()
				.PrintLine("input->clear(input->rdstate() ^ std::ios::eofbit);")
				.PrintLine("return true;").Up()
			.PrintLine('}')
			.PrintLine("return false;");
		out.PrintLine('}');
	}

	void LexCppGenerator::PeekFunc() {
		out.PrintLine("char ", peekFuncName, "() const {")
			.Down()
				.PrintLine("return static_cast<char>(input->peek());")
			.Up()
			.PrintLine('}');
	}

	void LexCppGenerator::SkipFunc() {
		out.PrintLine("void ", skipFuncName, "() {")
			.Down()
				.PrintLine("input->ignore();")
			.Up()
			.PrintLine('}');
	}

	void LexCppGenerator::LexerPrivateVars() {
		out.PrintLine("std::istream* input = nullptr;");
	}


	void LexCppGenerator::Generate() {
		Header();
	}
}