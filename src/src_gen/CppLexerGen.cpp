#include "src_gen/CppLexerGen.hpp"

#include "dfa/Automaton.hpp"
#include "cpp_utils.hpp"

#include "utils/char_utils.hpp"
#include "utils/string_utils.hpp"

#include <format>

namespace parsec::src_gen {
	namespace {
		class GenLexer {
		public:

			GenLexer(std::ostream& out, const fg::SymbolGrammar& inputSyntax, const ConfigStore& configs)
				: m_inputSyntax(inputSyntax), m_configs(configs), m_out(out) {
				m_dfa = dfa::Automaton(m_inputSyntax);
			}

			void operator()() {
				genTokenClass();
				genLexerClass();
			}

		private:
			void genTokenClass() {
				m_out << cpp_utils::makeEnum("TokenKinds", m_inputSyntax.symbols())
					<< '\n'
					<< "using Token = parsec::TokenBase<TokenKinds>;" << '\n'
					<< '\n'
					<< '\n';
			}
			
			void genStateTransitions(std::span<const dfa::StateTrans> transitions) {
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

			void genLexState(const dfa::State& state) {
				m_out << "\t" << std::format("state{}:", state.id()) << '\n';
				m_out << "\t\t" << "consume(scanner()->get());" << '\n';

				if(state.isStartState()) {
					m_out << "\t" << "start:" << '\n';
				}

				genStateTransitions(state.transitions());

				if(state.isAcceptState()) {
					m_out << "\t\t" << std::format("kind = TokenKinds::{};",
						state.acceptSymbol().value()
					) << '\n';
					m_out << "\t\t" << "goto accept;" << '\n';
				} else {
					m_out << "\t\t" << "error();" << '\n';
				}
			}

			void genLexStates() {
				if(m_dfa) {
					m_out << "\t\t" << "reset();" << '\n';
					m_out << "\t\t" << "goto start;" << '\n';
					m_out << '\n';
					
					for(const auto& state : m_dfa.states()) {
						genLexState(state);
						m_out << '\n';
					}
				} else {
					m_out << "\t\t" << "error();" << '\n';
					m_out << '\n';
				}
			}

			void genLexFunc() {
				if(const auto eofTok = m_configs.eofTokenName(); m_inputSyntax.contains(eofTok)) {
					m_out << "\t\t" << "if(scanner()->isEof()) {" << '\n';
					m_out << "\t\t\t" << std::format("kind = TokenKinds::{};", eofTok) << '\n';
					m_out << "\t\t\t" << "goto accept;" << '\n';
					m_out << "\t\t" << "}" << '\n';
				}
				
				genLexStates();

				m_out << "\t" << "accept:" << '\n';
				if(const auto wsTok = m_configs.wsTokenName(); m_inputSyntax.contains(wsTok)) {
					m_out << "\t\t" << std::format("if(kind == TokenKinds::{}) {{", wsTok) << '\n';
					m_out << "\t\t\t" << "goto reset;" << '\n';
					m_out << "\t\t" << "}" << '\n';
				}
				m_out << "\t\t" << "return kind;" << '\n';
			}

			void genLexerClass() {
				m_out << "class Lexer : public parsec::LexerBase<Token, TokenKinds> {" << '\n';
				m_out << "public:" << '\n';
				m_out << '\n';
				m_out<< "\t" << "using LexerBase::LexerBase;" << '\n';
				m_out << '\n';
				
				m_out << "private:" << '\n';
				m_out << "\t" << "TokenKinds onTokenParse() const override {" << '\n';
				m_out << "\t\t" << "TokenKinds kind = {};" << '\n';
				m_out << "\t" << "reset:" << '\n';
				
				genLexFunc();

				m_out << "\t" << "}" << '\n';
				m_out << "};" << '\n';
			}


			const fg::SymbolGrammar& m_inputSyntax;
			const ConfigStore& m_configs;
			std::ostream& m_out;

			dfa::Automaton m_dfa;
		};
	}


	void CppLexerGen::run(const fg::SymbolGrammar& inputSyntax, const ConfigStore& configs) {
		GenLexer(*m_out, inputSyntax, configs)();
	}
}