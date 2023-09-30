#include "LexCompiler.hpp"
#include "LexAutomaton.hpp"

#include <boost/tokenizer.hpp>

namespace {
	using namespace parsec;

	class AlignedPrinter {
	public:
		/** @{ */
		AlignedPrinter(std::ostream& out, std::size_t indent) noexcept
		 : out(&out), indent(indent)
		{ }
		/** @} */

		/** @{ */
		AlignedPrinter Down(std::size_t level = 1) const noexcept {
			return AlignedPrinter(*out, indent + level);
		}
		AlignedPrinter Up(std::size_t level = 1) const noexcept {
			return AlignedPrinter(*out, indent - level);
		}
		/** @} */

		/** @{ */
		template <typename... Args>
		AlignedPrinter& PrintLine(Args&&... args) {
			return Print(std::forward<Args>(args)..., '\n');
		}

		AlignedPrinter& PrintLine() {
			*out << '\n';
			return *this;
		}

		template <typename... Args>
		AlignedPrinter& Print(Args&&... args) {
			return RawPrint(MakeIndent(), std::forward<Args>(args)...);
		}

		template <typename... Args>
		AlignedPrinter& RawPrint(Args&&... args) {
			((*out << std::forward<Args>(args)), ...);
			return *this;
		}
		/** @} */

	private:
		/** @{ */
		std::string MakeIndent() const {
			constexpr static auto indentSize = 4;
			return std::string(indent * indentSize, ' ');
		}
		/** @} */

		std::ostream* out;
		std::size_t indent;
	};


	class CppGenerator {
	public:
		/** @{ */
		CppGenerator(const LexGrammar& grammar, std::ostream& out, std::size_t indent)
		 : grammar(grammar), automaton(grammar), out(out, indent)
		{ }
		/** @} */
		
		/** @{ */
		void Generate() {
			Header();
		}
		/** @} */

	private:
		/** @{ */
		constexpr static auto lexerClassName = "Lexer";
		constexpr static auto tokensEnumName = "Tokens";
		constexpr static auto eofTokenName = "Eof";

		constexpr static auto lexFuncName = "Lex";
		constexpr static auto peekFuncName = "PeekChar";
		constexpr static auto skipFuncName = "SkipChar";
		constexpr static auto eofFuncName = "IsEndOfFile";

		constexpr static auto startLabelName = "start";
		constexpr static auto errorLabelName = "error";
		/** @} */


		/** @{ */
		static std::string GetStateLabel(const LexAutomaton::State& state) {
			return (std::ostringstream() << "state" << state.GetID()).str();
		}

		static std::string GetTokenName(const LexToken& tok) {
			return ToIdentifier(tok.GetName());
		}

		static std::string ToIdentifier(const std::string& str) {
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
		/** @} */


		/** @{ */
		void Header() {
			TokensEnum();
			LexerClass();
		}

		void TokensEnum() {
			out.PrintLine("enum class ", tokensEnumName, " {");
			out.Down().Print(eofTokenName);
			for(const auto& tok : grammar.GetTokens()) {
				out.RawPrint(",\n");
				out.Down().Print(GetTokenName(tok));
			}
			out.PrintLine();
			out.PrintLine("};\n");
		}

		void LexerClass() {
			out.PrintLine("class ", lexerClassName, " {");
			LexerPublicSection();
			LexerPrivateSection();
			out.PrintLine("};");
		}
		/** @} */


		/** @{ */
		void LexerPublicSection() {
			out.PrintLine("public:");
			out = out.Down();
			LexerPublicMethods();
			out = out.Up();
		}

		void LexerPublicMethods() {
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

		void LexerCtor() {
			out.PrintLine(lexerClassName, "(std::istream& input) noexcept")
				.PrintLine(" : input(&input)")
				.PrintLine("{ }");
		}

		void LexerDtor() {
			out.PrintLine('~', lexerClassName, "() = default;");
		}

		void LexerMoveOps() {
			out.PrintLine(lexerClassName, '(', lexerClassName, "&&) = default;")
				.PrintLine(lexerClassName, "& operator=(", lexerClassName, "&&) = default;");
		}

		void LexerCopyOps() {
			out.PrintLine(lexerClassName, "(const ", lexerClassName, "&) = delete;")
				.PrintLine(lexerClassName, "& operator=(const ", lexerClassName, "&) = delete;");
		}

		void LexFunc() {
			out.PrintLine(tokensEnumName, ' ', lexFuncName, "() {");
			out = out.Down();
			out.PrintLine("if(!", eofFuncName, "()) {" );
			out.Down().PrintLine("goto ", startLabelName, ';');
			out.PrintLine('}');
			out.PrintLine("return ", tokensEnumName, "::", eofTokenName, ";\n");
			for(const auto& state : automaton.GetStates()) {
				State(state);
			}
			out.Up().PrintLine(errorLabelName, ": ");
			out.PrintLine("throw \"lex error\";");
			out = out.Up();
			out.PrintLine('}');
		}
		
		void State(const LexAutomaton::State& state) {
			out.Up().PrintLine(GetStateLabel(state), ':');
			out.PrintLine(skipFuncName, "();");
			if(&state == &automaton.GetStartState()) {
				out.Up().PrintLine(startLabelName, ':');
			}
			if(state.GetTransitionsCount() > 0) {
				Transitions(state.GetTransitions());
			}
			if(state.IsAccepting()) {
				out.PrintLine("return ", tokensEnumName, "::", GetTokenName(state.GetMatchedTokens().front()), ';');
			} else {
				out.PrintLine("goto ", errorLabelName, ';');
			}
			out.PrintLine();
		}

		void Transitions(const LexAutomaton::TransitionList& transitions) {
			out.PrintLine("if(!", eofFuncName, "()) {" );
			out.Down().PrintLine("switch(" , peekFuncName,"()) {");
			for(const auto& trans : transitions) {
				out.Down(2).PrintLine("case '", CharUtils::EscapeChar(trans.GetLabel()), "': ",
					"goto ", GetStateLabel(trans.GetNewState()), ';'
				);
			}
			out.Down().PrintLine('}');
			out.PrintLine('}');
		}
		/** @} */


		/** @{ */
		void LexerPrivateSection() {
			out.PrintLine("private:");
			out = out.Down();
			LexerPrivateMethods();
			LexerPrivateVars();
			out = out.Up();
		}

		void LexerPrivateMethods() {
			EofFunc();
			PeekFunc();
			out.PrintLine();
			SkipFunc();
			out.PrintLine();
		}

		void EofFunc() {
			out.PrintLine("bool ", eofFuncName, "() const {")
				.Down()
					.PrintLine("if(input->peek() == std::char_traits<char>::eof()) {")
					.Down()
						.PrintLine("input->clear(input->rdstate() ^ std::ios_base::eofbit);")
						.PrintLine("return true;")
					.Up()
					.PrintLine('}')
					.PrintLine("return false;")
				.Up()
				.PrintLine('}');
		}

		void PeekFunc() {
			out.PrintLine("char ", peekFuncName, "() const {")
				.Down()
					.PrintLine("return static_cast<char>(input->peek());")
				.Up()
				.PrintLine('}');
		}

		void SkipFunc() {
			out.PrintLine("void ", skipFuncName, "() {")
				.Down()
					.PrintLine("input->ignore();")
				.Up()
				.PrintLine('}');
		}

		void LexerPrivateVars() {
			out.PrintLine("std::istream* input;");
		}
		/** @} */

		const LexGrammar& grammar;
		LexAutomaton automaton;

		AlignedPrinter out;
	};
}

namespace parsec {
	void LexCompiler::Compile(std::size_t indent) {
		CppGenerator(*grammar, *out, indent)
			.Generate();
	}
}