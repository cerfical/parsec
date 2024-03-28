#include "src_gen/CppParserGen.hpp"
#include "src_gen/cpp_utils.hpp"

#include "fsm/Factory.hpp"
#include <format>

namespace parsec::src_gen {
	namespace {
		class GenParser {
		public:

			GenParser(std::ostream& out, const fg::SymbolGrammar& inputSyntax)
				: m_inputSyntax(inputSyntax), m_out(out) {
				m_slr = fsm::Factory::get()->makeSlr(m_inputSyntax);
			}

			void operator()() {
				genParseRulesEnum();
				genParserClass();
			}

		private:
			void genParseRulesEnum() {
				m_out << cpp_utils::makeEnum("ParseRules", m_inputSyntax.symbols());
				m_out << '\n';
			}

			void genProtected() {
				m_out << "protected:" << '\n';
				for(const auto& symbol : m_inputSyntax.symbols()) {
					m_out << "\t" << std::format("virtual void on{}(std::span<const Token> tokens) {{}}", symbol.value()) << '\n';
				}
				m_out << '\n';
			}

			void genStateReduce(const fsm::State& state, std::string_view indent) {
				/*if(state.inputMatch()) {
					m_out << indent << std::format("reduceTo(ParseRules::{0}, &Parser::on{0});",
						state.inputMatch().value()
					) << '\n';
				} else {
					m_out << indent << "error();" << '\n';
				}*/
			}

			void genStateShifts(const fsm::State& state) {
				bool first = true;
				for(const auto& trans : state.transitions()) {
					if(m_inputSyntax.contains(trans.label())) {
						continue;
					}

					if(std::exchange(first, false)) {
						m_out << "\t\t" << "switch(m_lexer.peek().kind()) {" << '\n';
					}

					m_out << "\t\t\t" << std::format("case TokenKinds::{}: shiftTo(&Parser::state{}); break;",
						trans.label().value(),
						trans.target()
					) << '\n';
				}
				
				if(!first) {
					m_out << "\t\t\t" << "default: {" << '\n';
					
					genStateReduce(state, "\t\t\t\t");
					
					m_out << "\t\t\t\t" << "break;" << '\n';
					m_out << "\t\t\t" << "}" << '\n';
					m_out << "\t\t" << "}" << '\n';
				} else {
					genStateReduce(state, "\t\t");
				}
			}

			void genStateGotos(const fsm::State& state) {
				bool first = true;
				for(const auto& trans : state.transitions()) {
					if(!m_inputSyntax.contains(trans.label())) {
						continue;
					}

					if(std::exchange(first, false)) {
						m_out << '\n';
						m_out << "\t\t" << "while(true) switch(m_reduceRule) {" << '\n';
					}

					m_out << "\t\t\t" << std::format("case ParseRules::{}: goTo(&Parser::state{}); break;",
						trans.label().value(),
						trans.target()
					) << '\n';
				}

				if(!first) {
					m_out << "\t\t\t" << "default: return;" << '\n';
					m_out << "\t\t" << "}" << '\n';
				}
			}

			void genParseState(const fsm::State& state) {
				m_out << "\t" << std::format("void state{}() {{", state.id()) << '\n';

				genStateShifts(state);
				genStateGotos(state);

				m_out << "\t" << "}" << '\n';
			}

			void genPrivate() {
				m_out << R"(private:
	using ParseHook = void (Parser::*)(std::span<const Token>);
	using StateFunc = void (Parser::*)();


	[[noreturn]] void error() const {
		throw parsec::ParseError("unexpected token", m_lexer.loc());
	}

	std::span<const Token> reduceRuleTokens() const noexcept {
		return std::span(m_parsedTokens.end() - m_reduceRuleTokenCount, m_reduceRuleTokenCount);
	}

	void shiftTo(StateFunc state) {
		m_parsedTokens.push_back(m_lexer.lex());
		(this->*state)();
		m_reduceRuleTokenCount++;
	}

	void goTo(StateFunc state) {
		reduce();
		(this->*state)();
	}

	void reduceTo(ParseRules rule, ParseHook hook) noexcept {
		m_reduceRule = rule;
		m_reduceHook = hook;
	}

	void reduce() {
		(this->*m_reduceHook)(reduceRuleTokens());
		m_parsedTokens.resize(m_parsedTokens.size() - m_reduceRuleTokenCount);
		m_reduceRuleTokenCount = 0;
	}

)";

				if(m_slr.startState()) {
					m_out << "\t" << "void startParse() {" << '\n';
					m_out << "\t\t" << std::format("state{}();", m_slr.startState().id()) << '\n';
					m_out << "\t\t" << "reduce();" << '\n';
					m_out << "\t" << "}" << '\n';
					m_out << '\n';
					m_out << '\n';
					
					for(const auto& state : m_slr.states()) {
						genParseState(state);
						m_out << '\n';
					}
				} else {
					m_out << "\t" << "void startParse() {}" << '\n';
					m_out << '\n';
				}

				m_out << R"(
	ParseHook m_reduceHook = {};
	std::size_t m_reduceRuleTokenCount = 0;
	ParseRules m_reduceRule = {};

	std::vector<Token> m_parsedTokens;
	Lexer m_lexer;
)";
			}

			void genParserClass() {
				m_out << R"(class Parser {
public:
	Parser() = default;

	Parser(const Parser&) = delete;
	Parser& operator=(const Parser&) = delete;

	Parser(Parser&&) = default;
	Parser& operator=(Parser&&) = default;

	void parse(std::istream& input) {
		m_lexer = Lexer(input);
		startParse();
	}

)";
				genProtected();
				genPrivate();
				m_out << "};" << '\n';
			}


			const fg::SymbolGrammar& m_inputSyntax;
			std::ostream& m_out;

			fsm::StateMachine m_slr;
		};
	}


	void CppParserGen::run(const fg::SymbolGrammar& inputSyntax, const ConfigStore&) {
		GenParser(*m_out, inputSyntax)();
	}
}