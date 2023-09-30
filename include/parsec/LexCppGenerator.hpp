#ifndef PARSEC_LEX_CPP_GENERATOR_HEADER
#define PARSEC_LEX_CPP_GENERATOR_HEADER

#include "LexGenerator.hpp"

namespace parsec {
	/**
	 * @brief Generates C++ source code for a lexical analyzer.
	 */
	class LexCppGenerator : public LexGenerator {
	public:
		/** @{ */
		/** @copybrief */
		LexCppGenerator() = default;

		/** @brief Construct and properly set up a new LexCppGenerator. */
		LexCppGenerator(const LexPatternList* patterns, std::ostream* out, std::size_t indent = 0)
		 : LexGenerator(patterns), out(out, indent)
		{ }

		/** @copybrief */
		~LexCppGenerator() = default;
		/** @} */
	
		/** @{ */
		LexCppGenerator(LexCppGenerator&&) = default;
		LexCppGenerator& operator=(LexCppGenerator&&) = default;
		/** @} */

		/** @{ */
		LexCppGenerator(const LexCppGenerator&) = delete;
		LexCppGenerator& operator=(const LexCppGenerator&) = delete;
		/** @} */

		/** @{ */
		void Generate() override;
		/** @} */

	private:
		class AlignedPrinter {
		public:
			/** @{ */
			AlignedPrinter() = default;

			AlignedPrinter(std::ostream* out, std::size_t indent) noexcept
			 : out(out), indent(indent)
			{ }
			/** @} */

			/** @{ */
			AlignedPrinter Down(std::size_t level = 1) const noexcept {
				return AlignedPrinter(out, indent + level);
			}
			AlignedPrinter Up(std::size_t level = 1) const noexcept {
				return AlignedPrinter(out, indent - level);
			}
			/** @} */

			/** @{ */
			template <typename... Args>
			AlignedPrinter& PrintLine(Args&&... args) {
				return Print(std::forward<Args>(args)..., '\n');
			}

			AlignedPrinter& PrintLine() {
				if(out) {
					*out << '\n';
				}
				return *this;
			}

			template <typename... Args>
			AlignedPrinter& Print(Args&&... args) {
				return RawPrint(MakeIndent(), std::forward<Args>(args)...);
			}

			template <typename... Args>
			AlignedPrinter& RawPrint(Args&&... args) {
				if(out) {
					((*out << std::forward<Args>(args)), ...);
				}
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

			std::ostream* out = nullptr;
			std::size_t indent = 0;
		};

		/** @{ */
		void OnPreState(LexState state) override;
		
		void OnPreTransitions() override;
		void OnTransition(LexState toState, char label) override;
		void OnPostTransitions() override;

		void OnAccept(const LexPattern& match) override;
		void OnReject() override;

		void OnPostState(LexState state) override;
		/** @} */

		/** @{ */
		void Header();

		void LocationClass();
		void TokensEnum();
		void TokenClass();
		void LexErrorClass();

		void LexerClass();
		/** @} */

		/** @{ */
		void LexerPublicSection();
		void LexerPublicMethods();

		void LexerCtors();
		void LexerDtor();

		void LexerMoveOps();
		void LexerCopyOps();

		void LexFunc();
		/** @} */

		/** @{ */
		void LexerPrivateSection();
		void LexerPrivateMethods();

		void EofFunc();
		void PeekCharFunc();
		void NextCharFunc();
		void MakeTokenFunc();

		void LexerPrivateVars();
		/** @} */

		AlignedPrinter out;
	};
}

#endif