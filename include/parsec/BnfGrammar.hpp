#ifndef PARSEC_BNF_GRAMMAR_HEADER
#define PARSEC_BNF_GRAMMAR_HEADER

#include "LexPattern.hpp"

namespace parsec {
	/**
	 * @brief Defines the syntax of some language.
	 */
	class BnfGrammar {
	public:
		/** @{ */
		/** @copybrief */
		BnfGrammar() = default;

		/** @copybrief */
		~BnfGrammar() = default;
		/** @} */

		/** @{ */
		BnfGrammar(BnfGrammar&&) = default;
		BnfGrammar& operator=(BnfGrammar&&) = default;
		/** @} */

		/** @{ */
		BnfGrammar(const BnfGrammar&) = delete;
		BnfGrammar& operator=(const BnfGrammar&) = delete;
		/** @} */

		/** @{ */
		/** @brief Add a new token to the language. */
		void AddToken(const std::string& name, const std::string& pattern) {
			tokens.emplace_back(name, pattern);
		}
		/** @} */

		/** @{ */
		/** @brief Get the list of tokens defined by the language. */
		const LexPatternList& GetTokens() const noexcept {
			return tokens;
		}

		/** @brief Get the number of tokens in the language. */
		std::size_t GetTokenCount() const noexcept {
			return tokens.size();
		}
		/** @} */

	private:
		LexPatternList tokens;
	};
}

#endif