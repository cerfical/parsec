#include "src_gen/CppCodeGen.hpp"
#include "src_gen/cpp_utils.hpp"

#include "dfa/Automaton.hpp"
#include "elr/Automaton.hpp"

#include "utils/char_utils.hpp"
#include "utils/string_utils.hpp"

#include <sstream>
#include <format>

namespace parsec::src_gen {
	namespace {
		class CppLexerGen {
		public:

			CppLexerGen(const fg::SymbolGrammar& tokens, const ConfigStore& configs)
				: m_tokens(tokens), m_configs(configs) {
				m_dfa = dfa::Automaton(tokens);
			}

			std::string run() {
				tokenClass();
				lexerClass();

				return std::move(m_out).str();
			}

		private:
			void tokenClass() {
				m_out << cpp_utils::makeEnum("TokenKinds", m_tokens.symbols())
					<< '\n'
					<< "using Token = parsec::TokenBase<TokenKinds>;" << '\n'
					<< '\n'
					<< '\n';
			}


			void lexerClass() {
				m_out << "class Lexer : public parsec::LexerBase<Token, TokenKinds> {" << '\n';
				m_out << "public:" << '\n';
				m_out << '\n';
				m_out << "\t" << "using LexerBase::LexerBase;" << '\n';
				m_out << '\n';

				m_out << "private:" << '\n';
				m_out << "\t" << "TokenKinds onTokenParse() const override {" << '\n';
				m_out << "\t\t" << "TokenKinds kind = {};" << '\n';
				m_out << "\t" << "reset:" << '\n';

				lexerClassLexFunc();

				m_out << "\t" << "}" << '\n';
				m_out << "};" << '\n';
			}


			void lexerClassLexFunc() {
				if(const auto eofTok = m_configs.eofTokenName(); m_tokens.contains(eofTok)) {
					m_out << "\t\t" << "if(scanner()->isEof()) {" << '\n';
					m_out << "\t\t\t" << std::format("kind = TokenKinds::{};", eofTok) << '\n';
					m_out << "\t\t\t" << "goto accept;" << '\n';
					m_out << "\t\t" << "}" << '\n';
				}

				lexStates();

				m_out << "\t" << "accept:" << '\n';
				if(const auto wsTok = m_configs.wsTokenName(); m_tokens.contains(wsTok)) {
					m_out << "\t\t" << std::format("if(kind == TokenKinds::{}) {{", wsTok) << '\n';
					m_out << "\t\t\t" << "goto reset;" << '\n';
					m_out << "\t\t" << "}" << '\n';
				}
				m_out << "\t\t" << "return kind;" << '\n';
			}


			void lexStates() {
				if(m_dfa) {
					m_out << "\t\t" << "reset();" << '\n';
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
				m_out << "\t\t" << "consume(scanner()->get());" << '\n';

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


			void lexTransitions(std::span<const dfa::StateTrans> transitions) {
				if(!transitions.empty()) {
					m_out << "\t\t" << "if(!scanner()->isEof()) switch(scanner()->peek()) {" << '\n';
					for(const auto& trans : transitions) {
						m_out << "\t\t\t" << std::format("case '{}': goto state{};",
							string_utils::escape(trans.label().value()),
							trans.target()
						) << '\n';
					}
					m_out << "\t\t" << "}" << '\n';
				}
			}


			const fg::SymbolGrammar& m_tokens;
			const ConfigStore& m_configs;
			
			std::ostringstream m_out;
			dfa::Automaton m_dfa;
		};



		class CppParserGen {
		public:

			CppParserGen(const fg::SymbolGrammar& syntax)
				: m_syntax(syntax) {
				m_elr = elr::Automaton(syntax);
			}

			std::string run() {
				parseRulesEnum();
				parserClass();

				return std::move(m_out).str();
			}

		private:
			void parseRulesEnum() {
				m_out << cpp_utils::makeEnum("ParseRules", m_syntax.symbols()) << '\n';
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


	[[noreturn]] void error() const {
		throw parsec::ParseError("unexpected token", m_lexer.loc());
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


			const fg::SymbolGrammar& m_syntax;
			
			std::ostringstream m_out;
			elr::Automaton m_elr;
		};
	}


	std::string CppCodeGen::onLexerGen(const fg::SymbolGrammar& tokens, const ConfigStore& configs) {
		return CppLexerGen(tokens, configs).run();
	}


	std::string CppCodeGen::onParserGen(const fg::SymbolGrammar& syntax, const ConfigStore& configs) {
		return CppParserGen(syntax).run();
	}


	std::string CppCodeGen::onPreambleGen(const ConfigStore& configs) {
		return "";
	}
}