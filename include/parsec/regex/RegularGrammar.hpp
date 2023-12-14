#ifndef PARSEC_REGEX_REGULAR_GRAMMAR_HEADER
#define PARSEC_REGEX_REGULAR_GRAMMAR_HEADER

#include "Pattern.hpp"

#include <gsl/narrow>
#include <deque>

namespace parsec::regex {
	
	/**
	 * @brief Describes a regular language using a set of string patterns.
	 */
	class RegularGrammar {
	public:

		using PatternList = std::deque<Pattern>;


		RegularGrammar() = default;

		RegularGrammar(RegularGrammar&&) = default;
		RegularGrammar& operator=(RegularGrammar&&) = default;

		RegularGrammar(const RegularGrammar&) = delete;
		RegularGrammar& operator=(const RegularGrammar&) = delete;


		/** @{ */
		/** @brief Compile a new pattern and insert it into the grammar. */
		void addPattern(std::string_view name, std::string_view regex) {
			m_patterns.emplace_back(name, regex, uniquePatternId());
		}


		/** @brief List of all patterns defined by the grammar. */
		const PatternList& patterns() const noexcept {
			return m_patterns;
		}
		/** @} */


	private:
		int uniquePatternId() const noexcept {
			return gsl::narrow_cast<int>(m_patterns.size());
		}

		PatternList m_patterns;
	};

}

#endif