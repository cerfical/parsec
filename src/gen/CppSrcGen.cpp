#include "gen/CppSrcGen.hpp"

#include "dfa/StateGen.hpp"
#include "utils/chars.hpp"

#include <boost/tokenizer.hpp>
#include <format>

namespace parsec::gen {
	namespace {
		/** @{ */
		constexpr std::string_view sWsSymbol = "ws";
		/** @} */


		/** @{ */
		constexpr std::string_view sLoc = {
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

		constexpr std::string_view sError = {
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

		constexpr std::string_view sTokens[] = {
R"(enum TokenKinds {
	Eof)", R"(
};
)"
		};

		constexpr std::string_view sToken = {
R"(class Token {
public:
	explicit Token(
		const std::string& text = "",
		TokenKinds kind = TokenKinds::Eof,
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


	bool eof() const noexcept {
		return m_kind == TokenKinds::Eof;
	}


private:
	std::string m_text;
	SourceLoc m_loc;
	TokenKinds m_kind;
};
)"
		};

		constexpr std::string_view sLexer[] = {
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
		if(m_tok) return *std::exchange(m_tok, {});
		return parseToken();
	}

	const Token& peek() const {
		if(!m_tok) m_tok = parseToken();
		return *m_tok;
	}

	bool eof() const {
		return peek().eof();
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


	bool inputEnd() const {
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

		if(inputEnd()) goto accept;
)", R"(
	accept:
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
		
		constexpr std::string_view sHelpers[] = {
R"(std::ostream& operator<<(std::ostream& out, TokenKinds tok) {
	switch(tok) {
		case TokenKinds::Eof: out << "Eof"; break;)", R"(
	}
	return out;
}

std::ostream& operator<<(std::ostream& out, const Token& tok) {
	out << "{ " << tok.kind() << ": \"" << tok.text() << "\" }";
	return out;
}
)"
		};
		/** @} */


		/** @{ */
		class RunImpl {
		public:
			/** @{ */
			RunImpl(std::ostream& out, const fg::Grammar& grammar) noexcept
				: m_grammar(grammar), m_out(out)
			{ }
			/** @} */


			/** @{ */
			void operator()() const {
				m_out << sLoc << '\n';
				m_out << sError << "\n\n";
				m_out << sTokens[0] << tokenKinds() << sTokens[1]<< '\n';
				m_out << sToken << "\n\n";
				m_out << sLexer[0] << lexStates() << sLexer[1] << "\n\n";
				m_out << sHelpers[0] << printTokenKinds() << sHelpers[1];
			}
			/** @} */


		private:
			/** @{ */
			static std::string capitalized(const std::string& str) {
				auto t = str;
				if(!str.empty()) {
					t.front() = std::toupper(t.front());
				}
				return t;
			}

			static std::string toPascalCase(const std::string& str) {
				const static boost::char_separator<char> delims(" \f\n\r\t\v-_");
				const auto toks = boost::tokenizer(str, delims);

				// capitalize each word in the string
				std::string id;
				for(const auto& tok : toks) {
					id += capitalized(tok);
				}
				return id;
			}
		
			static std::string toTokenName(const fg::Symbol& sym) {
				return toPascalCase(sym.name());
			}

			static std::string toStateLabel(int state) {
				return std::format("state{}", state);
			}
			/** @} */


			/** @{ */
			std::string printTokenKinds() const {
				std::string str;
				for(const auto tok : m_grammar.terminals()) {
					if(tok->name() != sWsSymbol) {
						str += std::format(
							"\n\t\tcase TokenKinds::{0}: out << \"{0}\"; break;",
							toTokenName(*tok)
						);
					}
				}
				return str;
			}

			std::string tokenKinds() const {
				std::string str;
				for(const auto tok : m_grammar.terminals()) {
					if(tok->name() != sWsSymbol) {
						str += ",\n\t" + toTokenName(*tok);
					}
				}
				return str;
			}

			std::string lexStates() const {
				const auto states = dfa::StateGen().run(m_grammar);
				std::string str;

				if(states.empty()) {
					str += "\t\tgoto error;\n";
					return str;
				}
				str += "\t\tgoto start;\n";

				for(const auto& s : states) {
					str += std::format("\n\t{}:\n", toStateLabel(s.id()));
					str += "\t\tnextChar();\n";
					if(s.id() == 0) {
						str += "\tstart:\n";
					}

					if(!s.transitions().empty()) {
						str += "\t\tif(!inputEnd()) switch(peekChar()) {\n";
						for(const auto& t : s.transitions()) {
							str += std::format(
								"\t\t\tcase '{}': goto {};\n",
								escapeChar(t.label()),
								toStateLabel(t.state())
							);
						}
						str += "\t\t}\n";
					}

					if(s.accepting()) {
						const auto matchedSym = s.matches().front().symbol();
						if(matchedSym->name() != sWsSymbol) {
							str += std::format("\t\tkind = TokenKinds::{};\n", toTokenName(*matchedSym));
							str += "\t\tgoto accept;\n";
						} else {
							str += "\t\tgoto reset;\n";
						}
					} else {
						str += "\t\tgoto error;\n";
					}
				}

				return str;
			}
			/** @} */


			/** @{ */
			const fg::Grammar& m_grammar;
			std::ostream& m_out;
			/** @} */
		};
		/** @} */
	}


	void CppSrcGen::run(const fg::Grammar& grammar) {
		RunImpl(*m_out, grammar)();
	}
}