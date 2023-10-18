#include "gen/CppLexerGenerator.hpp"
#include "utils/chars.hpp"

#include <boost/tokenizer.hpp>
#include <gsl/gsl>
#include <sstream>

namespace {
	using namespace parsec;
	using namespace boost;

	using str = gsl::czstring;

	/** @{ */
	constexpr str wsToken = "ws";

	constexpr struct {
		str colNo = "colNo";
		str colCount = "colCount";
		str lineNo = "lineNo";
		str linePos = "linePos";
		
		constexpr operator str() const noexcept {
			return "SourceLoc";
		}
	} sourceLoc;

	constexpr struct {
		str loc = "location";

		constexpr operator str() const noexcept {
			return "LexError";
		}
	} lexError;

	constexpr struct {
		str text = "text";
		str kind = "kind";
		str loc = "location";

		constexpr operator str() const noexcept {
			return "Token";
		}
	} token;

	constexpr struct {
		str input = "m_input";
		str buffer = "m_buffer";
		str pos = "m_pos";

		str eof = "eof";
		str peek = "peekChar";
		str next = "nextChar";
		str make = "makeToken";

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
				return "lex";
			}
		} lex;

		constexpr operator str() const noexcept {
			return "Lexer";
		}
	} lexer;

	constexpr struct {
		str eof = "Eof";

		constexpr operator str() const noexcept {
			return "Kinds";
		}
	} tokenKinds;
	/** @} */

	/** @{ */
	std::string capitalized(const std::string& str) {
		auto t = str;
		t.front() = std::toupper(t.front());
		return t;
	}

	std::string toPascalCase(const std::string& str) {
		const static char_separator<char> delims(" \f\n\r\t\v-_");
		const auto toks = tokenizer(str, delims);
		
		// capitalize each word in the string
		std::string id;
		for(const auto& tok : toks) {
			id += capitalized(tok);
		}
		return id;
	}

	std::string makeStateLabel(int state) {
		return (std::ostringstream() << lexer.lex.prefix << state).str();
	}

	std::string makeTokenKind(const fg::TokenRule& rule) {
		return toPascalCase(rule.name());
	}
	/** @} */
}

namespace parsec::gen {
	void CppLexerGenerator::generateHeader() {
		generateSourceLoc();
		m_out.printLine();
		generateLexError();
		m_out.printLine();
		generateToken();
		m_out.printLine();
		generateLexer();
	}


	void CppLexerGenerator::generateSourceLoc() {
		m_out.printLine("struct ", sourceLoc, " {").down()
			.printLine("int ", sourceLoc.colNo, " = { };")
			.printLine("int ", sourceLoc.colCount, " = { };")
			.printLine("int ", sourceLoc.lineNo, " = { };")
			.printLine("int ", sourceLoc.linePos, " = { };");
		m_out.printLine("};");
	}

	void CppLexerGenerator::generateLexError() {
		m_out.printLine("class ", lexError, " : public std::runtime_error {");
		m_out.printLine("public:");
		m_out.down()
			.printLine(lexError, "(const char* msg, const ", sourceLoc, "& loc)").down()
				.printLine(": runtime_error(msg), ", lexError.loc, "(loc)").up()
			.printLine("{ }\n")
			.printLine(lexError, "(const std::string& msg, const ", sourceLoc, "& loc)").down()
				.printLine(": runtime_error(msg), ", lexError.loc, "(loc)").up()
			.printLine("{ }\n")
			.printLine(sourceLoc, ' ', lexError.loc, ';');
		m_out.printLine("};");
	}

	void CppLexerGenerator::generateTokenKinds() {
		m_out.printLine("enum ", tokenKinds, " {");
		m_out.down().print(tokenKinds.eof);
		for(const auto& rule : inputGrammar().tokenRules()) {
			if(rule.name() != wsToken) {
				m_out.rawPrint(",\n").down().print(makeTokenKind(rule));
			}
		}
		m_out.printLine();
		m_out.printLine("};");
	}

	void CppLexerGenerator::generateToken() {
		m_out.printLine("struct ", token, " {");
		m_out = m_out.down();

		generateTokenKinds();
		m_out.printLine();

		m_out.printLine("std::string ", token.text, ';');
		m_out.printLine(sourceLoc, ' ', token.loc, ';');
		m_out.printLine(tokenKinds, ' ', token.kind, " = ", tokenKinds.eof, ';');

		m_out = m_out.up();
		m_out.printLine("};");
	}


	void CppLexerGenerator::generateLexer() {
		m_out.printLine("class ", lexer, " {");
		m_out = m_out.down();
		
		m_out.up().printLine("public:");
		generateLexerInterface();
		
		m_out.up().printLine("private:");
		generateLexerImpl();

		m_out = m_out.up();
		m_out.printLine("};");
	}


	void CppLexerGenerator::generateLexerInterface() {
		m_out.printLine(lexer, "() = default;");
		m_out.printLine(lexer, "(std::istream& input) noexcept");
		m_out.down().printLine(": ", lexer.input, "(&input)");
		m_out.printLine("{ }");

		m_out.printLine();
		
		m_out.printLine(lexer, '(', lexer, "&&) = default;");
		m_out.printLine(lexer, "& operator=(", lexer, "&&) = default;");

		m_out.printLine();
		
		m_out.printLine(lexer, "(const ", lexer, "&) = delete;");
		m_out.printLine(lexer, "& operator=(const ", lexer, "&) = delete;");

		m_out.printLine();
		generateLexerLex();
		m_out.printLine();
	}

	void CppLexerGenerator::generateLexerLex() {
		m_out.printLine(token, ' ', lexer.lex, "() {");
		m_out.printLine(lexer.lex.lex, ':');
		m_out = m_out.down();
		m_out.printLine(lexer.buffer, ".clear();");
		m_out.printLine(lexer.pos, '.', sourceLoc.colNo, " += ", lexer.pos, '.', sourceLoc.colCount, ';');
		m_out.printLine(lexer.pos, '.', sourceLoc.colCount, " = 0;");
		m_out.printLine("if(!", lexer.eof, "()) {" );
		m_out.down().printLine("goto ", lexer.lex.start, ';');
		m_out.printLine('}');
		m_out.printLine("return ", lexer.make, '(', token, "::", tokenKinds.eof, ");\n");
		
		generateStates();

		m_out.up().printLine(lexer.lex.error, ':');
		m_out.printLine("throw ", lexError, "(\"", lexer.lex.error.msg, "\", ", lexer.pos, ");");
		m_out = m_out.up();
		m_out.printLine('}');
	}


	void CppLexerGenerator::generateLexerImpl() {
		generateLexerEof();
		m_out.printLine();

		generateLexerPeek();
		m_out.printLine();

		generateLexerNext();
		m_out.printLine();
	
		generateLexerMake();
		m_out.printLine();

		m_out.printLine("std::istream* ", lexer.input, " = nullptr;");
		m_out.printLine("std::string ", lexer.buffer, ';');
		m_out.printLine(sourceLoc, ' ', lexer.pos, ';');
	}


	void CppLexerGenerator::generateLexerEof() {
		m_out.printLine("bool ", lexer.eof, "() const {").down()
			.printLine("if(", lexer.input, ") {").down()
				.printLine("if(", lexer.input, "->peek() != std::char_traits<char>::eof()) {").down()
					.printLine("return false;").up()
				.printLine('}')
				.printLine(lexer.input, "->clear(", lexer.input, "->rdstate() ^ std::ios::eofbit);").up()
			.printLine('}')
			.printLine("return true;");
		m_out.printLine('}');
	}

	void CppLexerGenerator::generateLexerPeek() {
		m_out.printLine("char ", lexer.peek, "() const {").down()
			.printLine("return static_cast<char>(", lexer.input, "->peek());");
		m_out.printLine('}');
	}

	void CppLexerGenerator::generateLexerNext() {
		m_out.printLine("void ", lexer.next, "() {").down()
			.printLine(lexer.pos, '.', sourceLoc.colCount,"++;")
			.printLine("if(", lexer.peek, "() == '\\n') {").down()
				.printLine(lexer.pos, '.', sourceLoc.linePos, " += ", lexer.pos, '.', sourceLoc.colNo, " + ", lexer.pos, '.', sourceLoc.colCount, ';')
				.printLine(lexer.pos, '.', sourceLoc.colNo, " = ", lexer.pos, '.', sourceLoc.colCount, " = 0;")
				.printLine(lexer.pos, '.', sourceLoc.lineNo, "++;").up()
			.printLine('}')
			.printLine(lexer.buffer, " += ", lexer.input, "->get();");
		m_out.printLine('}');
	}

	void CppLexerGenerator::generateLexerMake() {
		m_out.printLine(token, ' ', lexer.make, '(', token, "::", tokenKinds, " kind) const {").down()
			.printLine("return ", token, "{ ", lexer.buffer, ", ", lexer.pos, ", kind", " };");
		m_out.printLine('}');
	}


	void CppLexerGenerator::onLexerState(const LexerState& state) {
		m_out.up().printLine(makeStateLabel(state.id()), ':');
		m_out.printLine(lexer.next, "();");
		if(state.id() == 0) {
			m_out.up().printLine(lexer.lex.start, ':');
		}

		if(!state.transitions().empty()) {
			m_out.printLine("if(!", lexer.eof, "()) {");
			m_out = m_out.down();
			m_out.printLine("switch(", lexer.peek, "()) {");

			for(const auto& trans : state.transitions()) {
				m_out.down().printLine("case '", escapeChar(trans.label()), "': ",
					"goto ", makeStateLabel(trans.targetState()), ';'
				);
			}

			m_out.printLine('}');
			m_out = m_out.up();
			m_out.printLine('}');
		}

		if(state.accepting()) {
			if(state.match()->name() != wsToken) {
				m_out.printLine("return ", lexer.make, '(', token, "::", makeTokenKind(*state.match()), ");");
			} else {
				m_out.printLine("goto ", lexer.lex.lex, ';');
			}
		} else {
			m_out.printLine("goto ", lexer.lex.error, ';');
		}

		m_out.printLine();
	}

	void CppLexerGenerator::onCompile() {
		generateHeader();
	}
}