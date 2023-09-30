#ifndef PARSEC_LEX_PATTERN_HEADER
#define PARSEC_LEX_PATTERN_HEADER

#include "RegExNodes.hpp"

namespace parsec {
	/**
	 * @brief String pattern for a single token recognized by a lexer.
	 */
	class LexPattern {
	public:
		/** @{ */
		/** @brief Construct a new named LexPattern. */
		LexPattern(const std::string& name, const std::string& pattern);

		/** @copybrief */
		~LexPattern() = default;
		/** @} */

		/** @{ */
		LexPattern(LexPattern&&) = default;
		LexPattern& operator=(LexPattern&&) = default;
		/** @} */

		/** @{ */
		LexPattern(const LexPattern&) = delete;
		LexPattern& operator=(const LexPattern&) = delete;
		/** @} */

		/** @{ */
		/** @brief Get the name of the pattern. */
		const std::string& GetName() const noexcept {
			return name;
		}
		/** @brief Get the regex representation of the pattern. */
		const RegExNode& GetRegex() const noexcept {
			return *regex;
		}

		/** @brief Get the regex node marking the end of the pattern. */
		const RegExChar& GetEndMarker() const noexcept {
			return static_cast<const RegExChar&>(regex->GetRightChild());
		}

		/** @brief Get the string representation of the pattern. */
		const std::string& GetStr() const noexcept {
			return pattern;
		}
		/** @} */

	private:
		std::unique_ptr<RegExConcat> regex;
		std::string pattern;
		std::string name;
	};

	/** @brief List of @ref LexPattern "LexPatterns". */
	using LexPatternList = std::vector<LexPattern>;
}

#endif