#include "CppSrcGen.hpp"

#include "utils/chars.hpp"
#include "dfa/StateGen.hpp"
#include "lr/StateGen.hpp"

#include <ranges>
#include <format>

namespace parsec {
	namespace {
		class RunImpl {
		public:
			RunImpl(std::ostream& out, const Grammar& grammar) noexcept
				: m_grammar(grammar), m_out(out)
			{ }

			void operator()() {
				emitLocationSrc();
				m_out << '\n';
				
				emitErrorSrc();
				m_out << "\n\n";

				emitTokenKindsSrc();
				m_out << '\n';

				emitTokenSrc();
				m_out << "\n\n";
				
				emitLexerSrc();
				m_out << "\n\n";

				emitParserSrc();
				m_out << "\n\n";

				emitHelpersSrc();
			}

		private:
			/** @{ */
			static std::string makeTokenKind(const Symbol* sym) {
				return "TokenKinds::" + sym->name();
			}

			static std::string makeSymbolName(const Symbol* sym) {
				return "SymbolNames::" + sym->name();
			}

			static std::string makeParseHook(const Symbol* sym) {
				return "on" + sym->name() + "()";
			}
		
			static std::string makeStateLabel(int state) {
				return std::format("state{}", state);
			}

			static std::string makeStateFunc(int state) {
				return makeStateLabel(state) + "()";
			}
			/** @} */


			/** @{ */
			void emitEnumSrc(auto&& values, std::string_view name, std::string_view indent) {
				m_out << std::format("{}enum class {} {{", indent, name);

				bool firstValue = true;
				for(const auto& value : values) {
					if(!std::exchange(firstValue, false)) {
						m_out << ',';
					}
					m_out << std::format("\n\t{}{}", indent, value);
				}

				if(!firstValue) {
					m_out << '\n' << indent;
				} else {
					m_out << ' ';
				}
				m_out << "};\n";
			}


			void emitLocationSrc() {
				constexpr static std::string_view src = {
R"(class SourceLoc {
public:
	SourceLoc() = default;

	SourceLoc(
		int startCol,
		int colCount,
		int lineNo,
		int linePos
	) noexcept
		: m_startCol(startCol)
		, m_colCount(colCount)
		, m_lineNo(lineNo)
		, m_linePos(linePos)
	{ }


	SourceLoc(const SourceLoc&) = default;
	SourceLoc& operator=(const SourceLoc&) = default;


	int startCol() const noexcept {
		return m_startCol;
	}

	int endCol() const noexcept {
		return m_startCol + m_colCount;
	}

	int colCount() const noexcept {
		return m_colCount;
	}


	int lineNo() const noexcept {
		return m_lineNo;
	}

	int linePos() const noexcept {
		return m_linePos;
	}


private:
	int m_startCol = 0;
	int m_colCount = 0;
	int m_lineNo = 0;
	int m_linePos = 0;
};
)"
				};
				
				m_out << src;
			}

			void emitErrorSrc() {
				constexpr static std::string_view src = {
R"(class Error : public std::runtime_error {
public:
	Error(const std::string& msg, const SourceLoc& loc)
		: runtime_error(msg), m_loc(loc)
	{ }

	Error(const Error&) = default;
	Error& operator=(const Error&) = default;

	const SourceLoc& loc() const noexcept {
		return m_loc;
	}

private:
	SourceLoc m_loc;
};
)"
				};

				m_out << src;
			}


			void emitTokenKindsSrc() {
				emitEnumSrc(m_grammar.tokenSymbols() | std::views::transform(
					[](auto sym) { return sym->name(); }
					), "TokenKinds", ""
				);
			}

			void emitTokenSrc() {
				constexpr static std::string_view src = {
R"(class Token {
public:
	Token() = default;

	Token(
		const std::string& text,
		TokenKinds kind,
		const SourceLoc& loc = {}
	)
		: m_text(text)
		, m_loc(loc)
		, m_kind(kind)
	{ }


	Token(const Token&) = default;
	Token& operator=(const Token&) = default;

	Token(Token&&) = default;
	Token& operator=(Token&&) = default;


	const std::string& text() const noexcept {
		return m_text;
	}

	const SourceLoc& loc() const noexcept {
		return m_loc;
	}

	TokenKinds kind() const noexcept {
		return m_kind;
	}


	bool isEof() const noexcept {
		return m_kind == TokenKinds::Eof;
	}


private:
	std::string m_text;
	SourceLoc m_loc;
	TokenKinds m_kind = {};
};

using TokenList = std::vector<Token>;
)"
				};
			
				m_out << src;
			}


			void emitLexerSrc() {
				constexpr static std::string_view src[] = {
R"(class Lexer {
public:
	explicit Lexer(std::istream& input = std::cin) noexcept
		: m_input(&input)
	{ }


	Lexer(Lexer&&) = default;
	Lexer& operator=(Lexer&&) = default;

	Lexer(const Lexer&) = delete;
	Lexer& operator=(const Lexer&) = delete;


	Token lex() {
		if(m_tok) {
			return *std::exchange(m_tok, {});
		}
		return parseToken();
	}

	const Token& peek() const {
		if(!m_tok) {
			m_tok = parseToken();
		}
		return *m_tok;
	}

	bool isEof() const {
		return peek().isEof();
	}


	bool skipIf(TokenKinds kind) {
		if(peek().kind() == kind) {
			skip();
			return true;
		}
		return false;
	}

	bool skipIf(std::string_view text) {
		if(peek().text() == text) {
			skip();
			return true;
		}
		return false;
	}

	void skip() {
		lex();
	}


private:
	SourceLoc makeLoc(int startPos) const noexcept {
		return SourceLoc(
			startPos - m_linePos,
			m_pos - startPos,
			m_lineNo,
			m_linePos
		);
	}


	bool isInputEnd() const {
		if(m_input->peek() == std::char_traits<char>::eof()) {
			m_input->clear(m_input->rdstate() ^ std::ios::eofbit);
			return true;
		}
		return false;
	}

	char peekChar() const {
		return static_cast<char>(m_input->peek());
	}

	void nextChar() const {
		m_pos++;
		if(peekChar() == '\n') {
			m_linePos = m_pos;
			m_lineNo++;
		}
		m_buf += m_input->get();
	}


	Token parseToken() const {
	reset:
		const auto startPos = m_pos;
		
		TokenKinds kind = TokenKinds::Eof;
		m_buf.clear();

		if(isInputEnd()) {
			goto accept;
		}
)", R"(
	accept:
		if(kind == TokenKinds::Ws) {
			goto reset;
		}
		return Token(m_buf, kind, makeLoc(startPos));

	error:
		throw Error("ill-formed token", makeLoc(startPos));
	}


	mutable std::optional<Token> m_tok;

	mutable std::string m_buf;
	std::istream* m_input;

	mutable int m_linePos = 0;
	mutable int m_lineNo = 0;
	mutable int m_pos = 0;
};
)"
				};

				const auto states = dfa::StateGen().run(m_grammar);
				m_out << src[0];

				if(!states.empty()) {
					m_out << "\t\tgoto start;\n";
				} else {
					m_out << "\t\tgoto error;\n";
				}

				for(const auto& s : states) {
					m_out << std::format("\n\t{}:\n", makeStateLabel(s.id()));
					m_out << "\t\tnextChar();\n";
					if(s.isStart()) {
						m_out << "\tstart:";
					}

					if(s.transitions()) {
						m_out << "\n\t\tif(!isInputEnd()) switch(peekChar()) {\n";
						for(const auto t : s.transitions()) {
							m_out << std::format("\t\t\tcase '{}': goto {};\n",
								escapeChar(t->inputChar()),
								makeStateLabel(t->newState())
							);
						}
						m_out << "\t\t}\n";
					}

					m_out << '\n';
					if(s.isAccepting()) {
						const auto matchedSym = s.matches().front()->head();
						m_out << std::format("\t\tkind = {};\n", makeTokenKind(matchedSym));
						m_out << "\t\tgoto accept;\n";
					} else {
						m_out << "\t\tgoto error;\n";
					}
				}

				m_out << src[1];
			}
			/** @} */


			/** @{ */
			void emitSymbolNamesSrc(std::string_view indent) {
				emitEnumSrc(m_grammar.ruleSymbols() | std::views::transform(
					[](auto sym) { return sym->name(); }
					), "SymbolNames", indent
				);
			}

			void emitParseHooksSrc(std::string_view indent) {
				if(m_grammar.rules()) {
					m_out << '\n';
			
					for(const auto symbol : m_grammar.ruleSymbols()) {
						// skip generated symbols
						if(symbol->name().ends_with('_')) {
							continue;
						}

						m_out << indent << "virtual void " << makeParseHook(symbol) << " { }\n";
					}
				}
			}

			void emitStateReducesSrc(const auto& reduces, std::string_view indent) {
				if(reduces.empty()) {
					m_out << indent << "error();\n";
					return;
				}

				// take the first available reduce action, ignore the rest, if any
				const auto first = reduces.front();
				const auto symbol = first->newSymbol();
				
				const auto statesToPop = first->reducedStates() != 0 ? first->reducedStates() - 1 : 0;

				m_out << indent << std::format("reduce({}{}, {}, {});\n",
					statesToPop,
					symbol == m_grammar.root() ? " + 1" : "", // pop the start state
					first->consumedTokens(),
					makeSymbolName(symbol)
				);

				if(!symbol->name().ends_with('_')) {
					m_out << indent << makeParseHook(symbol) << ";\n";
					m_out << indent << "m_reducedTokens = 0;\n";
				}
				
				if(first->reducedStates()) {
					m_out << indent << "return;\n";
				} else {
					m_out << indent << "break;\n";
				}
			}

			void emitStateShiftsSrc(const lr::State& state, std::string_view indent) {
				m_out << indent << "switch(m_lexer.peek().kind()) {\n";
				
				for(const auto trans : state.shifts()) {
					m_out << indent<< "\tcase " << makeTokenKind(trans->inputSymbol());
					m_out << ": " << makeStateFunc(trans->newState()) << "; break;\n";
				}

				m_out << indent << "\tdefault: {\n";
				emitStateReducesSrc(state.reduces(), std::string(indent) + "\t\t");
				m_out << indent << "\t}\n";
				m_out << indent << "}\n";
			}

			void emitStateGotosSrc(const lr::State& state, std::string_view indent)  {
				if(state.gotos()) {
					m_out << indent << "while(m_statesToPop == 0) switch(m_reducedToSymbol) {\n";
					
					for(const auto& trans : state.gotos()) {
						m_out << indent << std::format("\tcase {}: {}; break;\n",
							makeSymbolName(trans->inputSymbol()),
							makeStateFunc(trans->newState())
						);
					}

					m_out << indent << "}\n";
				}

				m_out << indent << "--m_statesToPop;\n";
			}

			void emitParserSrc() {
				constexpr static std::string_view src[] = {
R"(class Parser {
public:
	Parser() = default;


	Parser(Parser&&) = default;
	Parser& operator=(Parser&&) = default;

	Parser(const Parser&) = default;
	Parser& operator=(const Parser&) = default;


	void parse(std::istream& input) {
		m_lexer = Lexer(input);
		state0();
	}


protected:)", R"(
	const Token& tokenAt(int i) const noexcept {
		return m_parsedTokens[m_parsedTokens.size() - m_reducedTokens + i];
	}

	int tokenCount() const noexcept {
		return m_reducedTokens;
	}

private:
)",R"(
	void reduce(int states, int tokens, SymbolNames symbol) {
		m_reducedToSymbol = symbol;
		m_statesToPop = states;
		m_reducedTokens += tokens;
	}

	[[noreturn]] void error() const {
		throw Error("unexpected token", m_lexer.peek().loc());
	}


	TokenList m_parsedTokens;
	int m_reducedTokens = 0;

	SymbolNames m_reducedToSymbol = {};
	int m_statesToPop = 0;

	Lexer m_lexer;
};
)"
				};

				m_out << src[0];
				emitParseHooksSrc("\t");
				m_out << src[1];
				emitSymbolNamesSrc("\t");
				m_out << "\n\n";
				
				for(const auto& state : lr::StateGen().run(m_grammar)) {
					m_out << "\tvoid " << makeStateFunc(state.id()) << " {\n";

					if(!state.isStart() && state.inputSymbol()->pattern()) {
						m_out << "\t\tm_parsedTokens.emplace_back(m_lexer.lex());\n";
					}

					emitStateShiftsSrc(state, "\t\t");
					m_out << '\n';
					emitStateGotosSrc(state, "\t\t");

					m_out << "\t}\n\n";
				}

				m_out << src[2];
			}
			/** @} */


			/** @{ */
			void emitHelpersSrc() {
				constexpr static std::string_view src[] = {
R"(std::ostream& operator<<(std::ostream& out, const SourceLoc& loc) {
	out << loc.lineNo() + 1 << ':' << loc.startCol() + 1 << '-' << loc.endCol() + 1;
	return out;
}

std::ostream& operator<<(std::ostream& out, TokenKinds tok) {
	switch(tok) {
)", R"(	}
	return out;
}

std::ostream& operator<<(std::ostream& out, const Token& tok) {
	out << "{ " << tok.kind() << ": \"" << tok.text() << "\" }";
	return out;
}
)", 
				};
				
				m_out << src[0];
				for(const auto symbol : m_grammar.tokenSymbols()) {
					m_out << std::format("\t\tcase {}: out << \"{}\"; break;\n",
						makeTokenKind(symbol),
						symbol->name()
					);
				}
				m_out << src[1];
			}
			/** @} */


			const Grammar& m_grammar;
			std::ostream& m_out;
		};
	}


	void CppSrcGen::run(const Grammar& grammar) {
		RunImpl(*m_out, grammar)();
	}
}