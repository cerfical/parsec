#ifndef PARSEC_CPP_LEX_GEN_HEADER
#define PARSEC_CPP_LEX_GEN_HEADER

#include "LexGen.hpp"

namespace parsec {
	/**
	 * @brief Generates C++ source code for a lexical analyzer.
	 */
	class LexCppGen : public LexGen {
	public:
		/** @{ */
		/** @brief Construct a defaulted generator that does nothing. */
		LexCppGen() = default;

		/** @brief Construct and properly set up a new LexCppGen. */
		LexCppGen(const Grammar* grammar, std::ostream* out)
		 : LexGen(grammar), m_out(out, 0)
		{ }
		/** @} */
	
		/** @{ */
		LexCppGen(LexCppGen&&) = default;
		LexCppGen& operator=(LexCppGen&&) = default;
		/** @} */

		/** @{ */
		LexCppGen(const LexCppGen&) = delete;
		LexCppGen& operator=(const LexCppGen&) = delete;
		/** @} */

		/** @{ */
		void Gen() override;
		/** @} */

	private:
		class AlignedPrinter {
		public:
			/** @{ */
			AlignedPrinter() = default;

			AlignedPrinter(std::ostream* out, std::size_t indent) noexcept
			 : m_out(out), m_indent(indent)
			{ }
			/** @} */

			/** @{ */
			AlignedPrinter Down() const noexcept {
				return AlignedPrinter(m_out, m_indent + 1);
			}
			AlignedPrinter Up() const noexcept {
				return AlignedPrinter(m_out, m_indent - 1);
			}
			/** @} */

			/** @{ */
			template <typename... Args>
			AlignedPrinter& PrintLine(Args&&... args) {
				return Print(std::forward<Args>(args)..., '\n');
			}

			AlignedPrinter& PrintLine() {
				return RawPrint('\n');
			}

			template <typename... Args>
			AlignedPrinter& Print(Args&&... args) {
				return RawPrint(MakeIndent(), std::forward<Args>(args)...);
			}

			template <typename... Args>
			AlignedPrinter& RawPrint(Args&&... args) {
				if(m_out) {
					((*m_out << std::forward<Args>(args)), ...);
				}
				return *this;
			}
			/** @} */

		private:
			/** @{ */
			std::string MakeIndent() const {
				return std::string(m_indent, '\t');
			}
			/** @} */

			std::ostream* m_out = nullptr;
			std::size_t m_indent = 0;
		};

		/** @{ */
		void OnPreState(StateID state) override;
		
		void OnPreTransitions() override;
		void OnTransition(StateID toState, char label) override;
		void OnPostTransitions() override;

		void OnAccept(const TokenPattern& match) override;
		void OnReject() override;

		void OnPostState(StateID state) override;
		/** @} */

		/** @{ */
		void GenHeader();

		void GenSourceLoc();
		void GenLexError();
		void GenTokenKinds();
		void GenToken();

		void GenLexer();
		/** @} */

		/** @{ */
		void GenLexerInterface();
		void GenLexerLex();
		/** @} */

		/** @{ */
		void GenLexerImpl();

		void GenLexerEof();
		void GenLexerPeek();
		void GenLexerNext();
		void GenLexerMake();
		/** @} */

		AlignedPrinter m_out;
	};
}

#endif