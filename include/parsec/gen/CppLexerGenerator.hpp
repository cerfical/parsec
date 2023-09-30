#ifndef PARSEC_GEN_CPP_LEXER_GENERATOR_HEADER
#define PARSEC_GEN_CPP_LEXER_GENERATOR_HEADER

#include "LexerGenerator.hpp"
#include <iostream>

namespace parsec::gen {
	/**
	 * @brief Generates C++ source code for a lexical analyzer.
	 */
	class CppLexerGenerator : public LexerGenerator {
	public:
		/** @{ */
		/** @brief Construct a generator that outputs to the @c std::cout. */
		CppLexerGenerator()
			: CppLexerGenerator(std::cout)
		{ }

		/** @brief Construct a generator that outputs to a @c std::ostream. */
		explicit CppLexerGenerator(std::ostream& out)
			: m_out(out, 0)
		{ }
		/** @} */

	private:
		class AlignedPrinter {
		public:
			/** @{ */
			AlignedPrinter(std::ostream& out, std::size_t indent) noexcept
				: m_out(&out), m_indent(indent)
			{ }
			/** @} */

			/** @{ */
			AlignedPrinter down() const noexcept {
				return AlignedPrinter(*m_out, m_indent + 1);
			}
			AlignedPrinter up() const noexcept {
				return AlignedPrinter(*m_out, m_indent - 1);
			}
			/** @} */

			/** @{ */
			template <typename... Args>
			AlignedPrinter& printLine(Args&&... args) {
				return print(std::forward<Args>(args)..., '\n');
			}

			AlignedPrinter& printLine() {
				return rawPrint('\n');
			}

			template <typename... Args>
			AlignedPrinter& print(Args&&... args) {
				return rawPrint(makeIndent(), std::forward<Args>(args)...);
			}

			template <typename... Args>
			AlignedPrinter& rawPrint(Args&&... args) {
				((*m_out << std::forward<Args>(args)), ...);
				return *this;
			}
			/** @} */

		private:
			/** @{ */
			std::string makeIndent() const {
				return std::string(m_indent, '\t');
			}
			/** @} */

			std::ostream* m_out = nullptr;
			std::size_t m_indent = 0;
		};

		/** @{ */
		void onLexerState(const LexerState& state) override;
		void onCompile() override;
		/** @} */

		/** @{ */
		void generateHeader();

		void generateSourceLoc();
		void generateLexError();
		void generateTokenKinds();
		void generateToken();

		void generateLexer();
		/** @} */

		/** @{ */
		void generateLexerInterface();
		void generateLexerLex();
		/** @} */

		/** @{ */
		void generateLexerImpl();

		void generateLexerEof();
		void generateLexerPeek();
		void generateLexerNext();
		void generateLexerMake();
		/** @} */

		AlignedPrinter m_out;
	};
}

#endif