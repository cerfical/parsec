#include "src_gen/CppCodeGen.hpp"

#include "dfa/Automaton.hpp"
#include "elr/Automaton.hpp"

#include "utils.hpp"

#include <format>

namespace parsec::src_gen {
	namespace {
		class CppLexerGen {
		public:

			CppLexerGen(const SymbolGrammar& tokens, const ConfigStore& configs, std::ostream& out)
				: m_tokens(tokens), m_configs(configs), m_out(out) {
				m_dfa = dfa::Automaton(tokens);
			}

			void run() {
				tokenKindsEnum();
				m_out << '\n';
				m_out << '\n';

				tokenClass();
				m_out << '\n';
				m_out << '\n';

				lexerClass();
			}

		private:
			void tokenKindsEnum() {
				m_out << cpp_utils::makeEnum("TokenKinds", m_tokens.symbols());
			}


			void tokenClass() {
				m_out << R"-(class Token {
public:

	Token() = default;

	Token(const std::string& text, TokenKinds kind, const SourceLoc& loc)
		: m_text(text), m_loc(loc), m_kind(kind) {}


	const std::string& text() const {
		return m_text;
	}

	const SourceLoc& loc() const {
		return m_loc;
	}

	TokenKinds kind() const {
		return m_kind;
	}


	template <TokenKinds tok>
	bool is() const {
		return kind() == tok;
	}


private:
	std::string m_text;
	SourceLoc m_loc;
	TokenKinds m_kind = {};
};

std::ostream& operator<<(std::ostream& out, const Token& tok) {
	return out << "(" << tok.kind() << ": \"" << tok.text() << "\")";
}
)-";
			}


			void lexerClass() {
				m_out << "class Lexer {" << '\n';
				
				lexerClassPublic();
				lexerClassPrivate();

				m_out << "};" << '\n';
			}


			void lexerClassPublic() {
				m_out << R"(public:

	Lexer() = default;

	explicit Lexer(std::istream& input)
		: m_input(&input) {}


	Lexer(const Lexer&) = delete;
	Lexer& operator=(const Lexer&) = delete;

	Lexer(Lexer&&) = default;
	Lexer& operator=(Lexer&&) = default;


	const Token& peek() {
		if(!m_token) {
			m_token = nextToken();
		}
		return *m_token;
	}

	Token lex() {
		if(!m_token) {
			m_token = nextToken();
		}

		Token tok = std::move(m_token.value());
		m_token.reset();
		return tok;
	}


	bool isEof() const {
		return isInputEnd();
	}

	SourceLoc loc() const {
		const auto colCount = m_inputPos - m_tokenStart;
		const auto startCol = m_tokenStart - m_inputLinePos;
		
		return SourceLoc(startCol, colCount, m_inputLineNo, m_inputLinePos);
	}


	bool skipIf(TokenKinds tok) {
		if(peek().kind() == tok) {
			skip();
			return true;
		}
		return false;
	}

	bool skipIf(std::string_view tok) {
		if(peek().text() == tok) {
			skip();
			return true;
		}
		return false;
	}

	void skip() {
		lex();
	}


)";
			}


			void lexerClassPrivate() {
				m_out << R"(private:
	consteval static unsigned char uchar(char ch) noexcept {
		return static_cast<unsigned char>(ch);
	}

	Token nextToken() {
		const auto kind = parseToken();
		return Token(m_tokenText, kind, loc());
	}

)";

				lexerClassParseTokenFunc();
				m_out << '\n';

				m_out << R"(	int getChar() {
		if(isInputEnd()) {
			return std::char_traits<char>::eof();
		}

		const auto ch = m_input->get();
		if(ch == uchar('\n')) {
			m_inputLinePos = m_inputPos;
			m_inputLineNo++;
		}
		m_inputPos++;
		return ch;
	}

	int peekChar() {
		if(isInputEnd()) {
			return std::char_traits<char>::eof();
		}
		return m_input->peek();
	}

	bool isInputEnd() const {
		if(!m_input) {
			return true;
		}

		if(m_input->peek() == std::char_traits<char>::eof()) {
			m_input->clear(m_input->rdstate() ^ std::ios::eofbit);
			return true;
		}
		return false;
	}

	[[noreturn]]
	void error() {
		throw ParseError(isInputEnd() ? "unexpected end of file" : "malformed token", loc());
	}

	std::istream* m_input = {};
	int m_inputPos = 0;
	int m_inputLineNo = 0;
	int m_inputLinePos = 0;

	std::optional<Token> m_token;
	std::string m_tokenText;
	int m_tokenStart = {};
)";
			}


			void lexerClassParseTokenFunc() {
				m_out << "\t" << "TokenKinds parseToken() {" << '\n';
				m_out << "\t\t" << "TokenKinds kind = {};" << '\n';
				m_out << "\t" << "reset:" << '\n';

				if(const auto& eof = eofToken()) {
					m_out << "\t\t" << "if(isInputEnd()) {" << '\n';
					m_out << "\t\t\t" << std::format("kind = TokenKinds::{};", eof.value()) << '\n';
					m_out << "\t\t\t" << "goto accept;" << '\n';
					m_out << "\t\t" << "}" << '\n';
				}
				m_out << "\t\t" << "m_tokenStart = m_inputPos;" << '\n';
				m_out << "\t\t" << "m_tokenText.clear();" << '\n';

				lexStates();

				m_out << "\t" << "accept:" << '\n';
				if(const auto& ws = wsToken()) {
					m_out << "\t\t" << std::format("if(kind == TokenKinds::{}) {{", ws.value()) << '\n';
					m_out << "\t\t\t" << "goto reset;" << '\n';
					m_out << "\t\t" << "}" << '\n';
				}
				m_out << "\t\t" << "return kind;" << '\n';
				m_out << "\t" << "}" << '\n';
			}


			void lexStates() {
				if(m_dfa) {
					m_out << "\t\t" << "goto start;" << '\n';
					m_out << '\n';

					for(const auto& state : m_dfa.states()) {
						lexState(state);
						m_out << '\n';
					}
				} else {
					m_out << "\t\t" << "error();" << '\n';
					m_out << '\n';
				}
			}


			void lexState(const dfa::State& state) {
				m_out << "\t" << std::format("state{}:", state.id()) << '\n';
				m_out << "\t\t" << "m_tokenText += getChar();" << '\n';

				if(state.isStartState()) {
					m_out << "\t" << "start:" << '\n';
				}

				lexTransitions(state.transitions());

				if(state.isMatchState()) {
					m_out << "\t\t" << std::format("kind = TokenKinds::{};",
						state.matchedRule().value()
					) << '\n';
					m_out << "\t\t" << "goto accept;" << '\n';
				} else {
					m_out << "\t\t" << "error();" << '\n';
				}
			}


			void lexTransitions(const std::vector<dfa::StateTrans>& transitions) {
				if(!transitions.empty()) {
					m_out << "\t\t" << "switch(peekChar()) {" << '\n';
					for(const auto& trans : transitions) {
						m_out << "\t\t\t" << std::format("case uchar('{}'): goto state{};",
							string_utils::escape(trans.label().value()),
							trans.target()
						) << '\n';
					}
					m_out << "\t\t" << "}" << '\n';
				}
			}


			const Symbol& eofToken() const {
				return m_tokens.resolve(m_configs.eofTokenName()).head();
			}


			const Symbol& wsToken() const {
				return m_tokens.resolve(m_configs.wsTokenName()).head();
			}


			const SymbolGrammar& m_tokens;
			const ConfigStore& m_configs;
			
			std::ostream& m_out;
			dfa::Automaton m_dfa;
		};



		class CppParserGen {
		public:

			CppParserGen(const SymbolGrammar& syntax, std::ostream& out)
				: m_syntax(syntax), m_out(out) {
				m_elr = elr::Automaton(syntax);
			}

			void run() {
				parseRulesEnum();
				m_out << '\n';
				m_out << '\n';

				parserClass();
			}

		private:
			void parseRulesEnum() {
				m_out << cpp_utils::makeEnum("ParseRules", m_syntax.symbols());
			}


			void parserClass() {
				m_out << R"(class Parser {
public:

	Parser() = default;

	Parser(const Parser&) = delete;
	Parser& operator=(const Parser&) = delete;

	Parser(Parser&&) = default;
	Parser& operator=(Parser&&) = default;


	void parse(std::string_view str) {
		auto in = std::istringstream(std::string(str));
		parse(in);
	}

	void parse(std::istream& input) {
		m_lexer = Lexer(input);
		startParse();
	}


)";

				parserClassProtected();
				parserClassPrivate();
				
				m_out << "};" << '\n';
			}


			void parserClassProtected() {
				m_out << "protected:" << '\n';
				for(const auto& symbol : m_syntax.symbols()) {
					m_out << "\t" << std::format("virtual void on{}(std::span<const Token> tokens) {{}}", symbol.value()) << '\n';
				}
				m_out << '\n';
			}


			void parserClassPrivate() {
				m_out << R"(private:
	using ParseHook = void (Parser::*)(std::span<const Token>);
	using StateFunc = void (Parser::*)();


	[[noreturn]] void error() {
		throw ParseError("unexpected token", m_lexer.loc());
	}

	void shiftState(StateFunc state) {
		m_parsedTokens.push_back(m_lexer.lex());
		(this->*state)();
		m_reduceTokenCount++;
	}

	void gotoState(StateFunc state) {
		(this->*state)();
	}

	void startReduce(ParseRules rule, ParseHook hook, int backLink) noexcept {
		m_reduceRule = rule;
		m_reduceHook = hook;
		m_reduceBackLink = backLink;
	}

	bool reduce(const int backLinks[]) {
		m_reduceBackLink = backLinks[m_reduceBackLink];
		if(m_reduceBackLink == -1) {
			(this->*m_reduceHook)(std::span(m_parsedTokens.end() - m_reduceTokenCount, m_reduceTokenCount));
			m_parsedTokens.resize(m_parsedTokens.size() - m_reduceTokenCount);
			m_reduceTokenCount = 0;
			return true;
		}
		return false;
	}

)";

				if(m_elr) {
					m_out << "\t" << "void startParse() {" << '\n';
					m_out << "\t\t" << std::format("state{}();", m_elr.startState().id()) << '\n';
					m_out << "\t" << "}" << '\n';
					m_out << '\n';
					m_out << '\n';

					for(const auto& state : m_elr.states()) {
						parserState(state);
						m_out << '\n';
					}
				} else {
					m_out << "\t" << "void startParse() {}" << '\n';
					m_out << '\n';
				}

				m_out << R"(
	ParseHook m_reduceHook = {};
	std::size_t m_reduceTokenCount = 0;
	ParseRules m_reduceRule = {};
	int m_reduceBackLink = -1;

	std::vector<Token> m_parsedTokens;
	Lexer m_lexer;
)";
			}


			void parserState(const elr::State& state) {
				m_out << "\t" << std::format("void state{}() {{", state.id()) << '\n';

				parserStateBackLinks(state);
				parserStateShifts(state);
				parserStateGotos(state);

				m_out << "\t" << "}" << '\n';
			}


			void parserStateShifts(const elr::State& state) {
				m_out << "\t\t" << "switch(m_lexer.peek().kind()) {" << '\n';
				for(const auto& trans : state.shifts()) {
					m_out << "\t\t\t" << std::format("case TokenKinds::{}: shiftState(&Parser::state{}); break;",
						trans.label().value(),
						trans.target()
					) << '\n';
				}

				m_out << "\t\t\t" << "default: " << (state.isReduceState()
					? std::format("startReduce(ParseRules::{0}, &Parser::on{0}, {1});",
						state.reduction().reduceRule().value(),
						state.reduction().backLink())
					: "error();")
					<< " break;" << '\n';

				m_out << "\t\t" << "}" << '\n';
			}


			void parserStateGotos(const elr::State& state) {
				m_out << "\t\t" << "while(reduce(backLinks)) switch(m_reduceRule) {" << '\n';

				for(const auto& trans : state.gotos()) {
					m_out << "\t\t\t" << std::format("case ParseRules::{}: gotoState(&Parser::state{}); break;",
						trans.label().value(),
						trans.target()
					) << '\n';
				}

				m_out << "\t\t\t" << "default: break;" << '\n';
				m_out << "\t\t" << "}" << '\n';
			}


			void parserStateBackLinks(const elr::State& state) {
				m_out << "\t\t" << "static const int backLinks[] = {";
				for(bool first = true; const auto & backLink : state.backLinks()) {
					if(!first) {
						m_out << ',';
					} else {
						first = false;
					}
					m_out << ' ' << backLink;
				}
				m_out << " };" << '\n';
			}


			const SymbolGrammar& m_syntax;
			
			std::ostream& m_out;
			elr::Automaton m_elr;
		};
	}


	void CppCodeGen::onLexerGen(const SymbolGrammar& tokens, const ConfigStore& configs) {
		CppLexerGen(tokens, configs, output()).run();

		output() << '\n';
		output() << '\n';
	}


	void CppCodeGen::onParserGen(const SymbolGrammar& syntax, const ConfigStore& configs) {
		CppParserGen(syntax, output()).run();
	}


	void CppCodeGen::onPreambleGen(const ConfigStore& configs) {
		output() << R"(class SourceLoc {
public:

	SourceLoc() = default;

	SourceLoc(int startCol, int colCount, int lineNo, int linePos)
		: m_startCol(startCol), m_colCount(colCount), m_lineNo(lineNo), m_linePos(linePos) {}


	int linePos() const {
		return m_linePos;
	}

	int lineNo() const {
		return m_lineNo;
	}

	int colCount() const {
		return m_colCount;
	}

	int startCol() const {
		return m_startCol;
	}


	int endCol() const {
		return startCol() + colCount();
	}

	int pos() const {
		return linePos() + startCol();
	}


private:
	int m_startCol = {};
	int m_colCount = {};
	int m_lineNo = {};
	int m_linePos = {};
};

std::ostream& operator<<(std::ostream& out, const SourceLoc& loc) {
	out << '(' << loc.lineNo() + 1 << ": " << loc.startCol() + 1;
	if(loc.colCount()) {
		out << '-' << (loc.endCol() - 1) + 1;
	}
	out << ')';
	return out;
}


class ParseError : public std::runtime_error {
public:
		
	ParseError(const std::string& msg, const SourceLoc& loc)
		: runtime_error(msg), m_loc(loc) {}

	const SourceLoc& loc() const {
		return m_loc;
	}

private:
	SourceLoc m_loc;
};
)";

		output() << '\n';
		output() << '\n';
	}
}