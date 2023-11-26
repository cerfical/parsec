#include "core/CppSrcGen.hpp"

#include "utils/Chars.hpp"
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
				emitToken();
				m_out << "\n\n";
				emitLexer();
				
				if(m_grammar.root()->rules()) {
					m_out << "\n\n";
					emitParser();
				}
			}

		private:
			/** @{ */
			static std::string makeTokenKind(const Symbol* symbol) {
				return "TokenKinds::" + symbol->name();
			}

			static std::string makeParseRule(const Symbol* symbol) {
				return "ParseRules::" + symbol->name();
			}

			static std::string makeParseHook(const Symbol* symbol) {
				return "on" + symbol->name();
			}
		
			static std::string makeStateLabel(int state) {
				return std::format("state{}", state);
			}

			static std::string makeStateFunc(int state) {
				return makeStateLabel(state) + "()";
			}
			/** @} */


			/** @{ */
			void emitEnum(std::string_view name, auto&& values) {
				m_out << std::format("enum class {} {{\n", name);
				m_out << "\tNone_";

				for(const auto& value : values) {
					m_out << std::format(",\n\t{}", value);
				}

				m_out << "\n};\n";
			}

			void emitToken() {
				// generate enumeration listing available token types
				emitEnum("TokenKinds",
					m_grammar.tokenSymbols() | std::views::transform(
						[](const auto symbol) -> const std::string& { return symbol->name(); }
					)
				);
				m_out << '\n';

				// helper function to present token types in a readable form
				m_out << "std::ostream& operator<<(std::ostream& out, TokenKinds tok) {\n";
				m_out << "\tswitch(tok) {\n";

				for(const auto tok : m_grammar.tokenSymbols()) {
					m_out << std::format("\t\tcase {}: out << \"{}\"; break;\n",
						makeTokenKind(tok),
						tok->name()
					);
				}

				m_out << "\t}\n";
				m_out << "\treturn out;\n";
				m_out << "}\n";

				m_out << '\n';

				// Token is just a type alias
				m_out << "using Token = parsec::TokenBase<TokenKinds>;\n";
			}

			void emitLexer() {
				m_out << R"(class Lexer : public parsec::LexerBase<Token, TokenKinds> {
public:
	using LexerBase::LexerBase;

private:
	TokenKinds onTokenParse() const override {
		TokenKinds kind = {};
	reset:
		if(scanner()->isEof()) {
			kind = TokenKinds::Eof;
			goto accept;
		}
)";

				const auto dfaStates = dfa::StateGen().run(m_grammar);
				if(!dfaStates.empty()) {
					m_out << "\t\treset();\n";
					m_out << "\t\tgoto start;\n";
				} else {
					m_out << "\t\terror();\n";
				}

				for(const auto& state : dfaStates) {
					m_out << std::format("\n\t{}:\n", makeStateLabel(state.id()));
					m_out << "\t\tconsume(scanner()->get());\n";
					if(state.isStart()) {
						m_out << "\tstart:";
					}

					// generate source code to perform state gransitions if there are any
					if(state.transitions()) {
						m_out << "\n\t\tif(!scanner()->isEof()) switch(scanner()->peek()) {\n";
						for(const auto transition : state.transitions()) {
							m_out << std::format("\t\t\tcase '{}': goto {};\n",
								utils::Chars::escape(transition->inputChar()),
								makeStateLabel(transition->newState())
							);
						}
						m_out << "\t\t}\n";
					}
					m_out << '\n';

					// action to take when the lexer cannot perform any state transitions
					if(state.isAccepting()) {
						// just use the match with the lowest id and ignore the rest
						const auto match = *std::ranges::min_element(state.matches(),
							[](const auto m1, const auto m2) {
								return m1->id() < m2->id();
							}
						);
						m_out << std::format("\t\tkind = {};\n\t\tgoto accept;\n", makeTokenKind(match->head()));
					} else {
						m_out << "\t\terror();\n";
					}
				}


				m_out << R"(
	accept:
		if(kind == TokenKinds::Ws) {
			goto reset;
		}
		return kind;
	}
};
)";
			}
			/** @} */


			/** @{ */
			void emitParseRules() {
				// enumeration to specify parsed rule names
				emitEnum("ParseRules", m_grammar.ruleSymbols() | std::views::transform(
					[](const auto symbol) -> const std::string& { return symbol->name(); }
				));
				m_out << '\n';
			}

			void emitParseHooks() {
				for(const auto symbol : m_grammar.ruleSymbols()) {
					if(symbol->isReserved()) {
						continue;
					}
					m_out << '\t' << std::format("virtual void {}() {{ }}\n",
						makeParseHook(symbol)
					);
				}
				m_out << '\n';
			}
			
			void emitParserInterface() {
				// public interface
				m_out << R"(class Parser {
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


protected:
)";

				emitParseHooks();

				// protected helper functions
				m_out << R"(	const Token& tokenAt(int i) const noexcept {
		return m_parsedTokens[m_parsedTokens.size() - ruleTokens() + i];
	}

	int ruleTokens() const noexcept {
		return m_reduceRuleTokens;
	}

)";
			}


			void emitReduces(const lr::State& state) {
				constexpr static auto indent = "\t\t\t\t";

				const auto reduces = state.reduces();
				if(!reduces) {
					m_out << indent << "error();\n";
					return;
				}

				// take the first available reduce action, ignore the rest, if any
				const auto reduce = *std::ranges::min_element(reduces, [](const auto r1, const auto r2) {
					return r1->newSymbol()->id() < r2->newSymbol()->id();
				});
				const auto symbol = reduce->newSymbol();

				m_out << indent << std::format("initiateReduce({}, {}, {}, {});\n",
					makeParseRule(symbol),
					reduce->reducedStates(),
					reduce->consumedTokens(),
					!symbol->isReserved()
						? "&Parser::" + makeParseHook(symbol)
						: "{}"
				);
				m_out << indent << "break;\n";
			}

			void emitShifts(const lr::State& state) {
				constexpr static auto indent = "\t\t";
				m_out << "\tshifts:\n";
				
				m_out << indent << "switch(m_lexer.peek().kind()) {\n";
				for(const auto shift : state.shifts()) {
					m_out << indent << '\t' << std::format("case {}: {}; break;\n",
						makeTokenKind(shift->inputSymbol()),
						makeStateFunc(shift->newState())
					);
				}
				m_out << indent << "\tdefault: {\n";

				emitReduces(state);
				
				m_out << indent << "\t}\n";
				m_out << indent << "}\n";
			}

			void emitGotos(const lr::State& state) {
				constexpr static auto indent = "\t\t";
				if(state.gotos()) {
					m_out << "\tgotos:\n";
					m_out << indent << "while(m_statesToPop == 0) switch(reduce()) {\n";
					
					// generate goto table for the state
					for(const auto transition : state.gotos()) {
						m_out << indent << '\t' << std::format("case {}: {}; break;\n",
							makeParseRule(transition->inputSymbol()),
							makeStateFunc(transition->newState())
						);
					}

					m_out << indent << "}\n\n";
				}
			}

			void emitParserState(const lr::State& state) {
				m_out << '\t' << std::format("void {} {{", makeStateFunc(state.id())) << '\n';

				// generate statements to execute when entering and exiting the state
				std::string statePush, statePop;
				if(state.inputSymbol()) {
					if(state.inputSymbol()->pattern()) {
						if(!state.inputSymbol()->isReserved()) {
							// store named tokens in the buffer
							statePush += "\t\tm_parsedTokens.emplace_back(m_lexer.lex());\n";
						} else {
							// and just skip unnamed tokens
							statePush += "\t\tm_lexer.skip();\n";
						}
					}

					// preserve information about reductions on reserved rules 
					if(state.inputSymbol()->rules() && state.inputSymbol()->isReserved()) {
						statePush += "\t\tconst int ruleTokens = m_reduceRuleTokens;\n";
						statePop += "\t\tm_reduceRuleTokens += ruleTokens;\n";
					}
				}
				statePop += "\t\tm_statesToPop--;\n";

				if(!statePush.empty()) {
					m_out << '\t' << "push:" << '\n' << statePush << '\n';
				}

				emitShifts(state);
				m_out << '\n';
				emitGotos(state);

				m_out << '\t' << "pop:" << '\n' << statePop;
				m_out << '\t' << '}' << '\n' << '\n';
			}

			void emitParserData() {
				m_out << R"(

	std::vector<Token> m_parsedTokens;

	void (Parser::*m_reduceHook)() = {};
	ParseRules m_reduceRule = {};
	int m_reduceRuleTokens = 0;
	int m_statesToPop = 0;

	Lexer m_lexer;
};
)";
			}

			void emitParserImpl() {
				m_out << R"(private:
	[[noreturn]] void error() const {
		throw parsec::Error("unexpected token", m_lexer.loc());
	}

	void initiateReduce(ParseRules rule, int states, int tokens, void (Parser::*hook)()) {
		m_statesToPop = states;
		m_reduceRuleTokens = tokens;
		m_reduceRule = rule;
		m_reduceHook = hook;
	}

	ParseRules reduce() {
		if(const auto reduceHook = std::exchange(m_reduceHook, {})) {
			(this->*reduceHook)();
			for(int i = 0; i < m_reduceRuleTokens; i++) {
				m_parsedTokens.pop_back();
			}
			m_reduceRuleTokens = 0;
		}

		if(m_reduceRule == ParseRules::Root) {
			m_statesToPop = -1;
			return {};
		}

		return std::exchange(m_reduceRule, {});
	}

)";

				for(const auto& state : lr::StateGen().run(m_grammar)) {
					emitParserState(state);
				}
			}

			void emitParser() {
				emitParseRules();
				emitParserInterface();
				emitParserImpl();
				emitParserData();
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