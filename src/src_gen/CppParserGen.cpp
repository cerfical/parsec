#include "src_gen/CppParserGen.hpp"
#include "src_gen/cpp_utils.hpp"

#include "elr/Automaton.hpp"
#include <format>

namespace parsec::src_gen {
	namespace {
		class CppParserGenImpl {
		public:

			CppParserGenImpl(std::ostream& out, const fg::SymbolGrammar& inputSyntax)
				: m_inputSyntax(inputSyntax), m_out(out) {
				m_elr = elr::Automaton(inputSyntax);
			}


			void run() {
				parseRulesEnum();
				parserClass();
			}


		private:
			void parseRulesEnum() {
				m_out << cpp_utils::makeEnum("ParseRules", m_inputSyntax.symbols()) << '\n';
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
				for(const auto& symbol : m_inputSyntax.symbols()) {
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
				for(bool first = true; const auto& backLink : state.backLinks()) {
					if(!first) {
						m_out << ',';
					} else {
						first = false;
					}
					m_out << ' ' << backLink;
				}
				m_out << " };" << '\n';
			}


			const fg::SymbolGrammar& m_inputSyntax;
			std::ostream& m_out;

			elr::Automaton m_elr;
		};
	}


	void CppParserGen::run(const fg::SymbolGrammar& inputSyntax, const ConfigStore&) {
		CppParserGenImpl(*m_out, inputSyntax).run();
	}
}