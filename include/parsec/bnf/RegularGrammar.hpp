#ifndef PARSEC_BNF_REGULAR_GRAMMAR_HEADER
#define PARSEC_BNF_REGULAR_GRAMMAR_HEADER

#include "RegularPattern.hpp"

#include <gsl/narrow>
#include <deque>

namespace parsec::bnf {
	
	/**
	 * @brief Describes a regular language using a set of string patterns.
	*/
	class RegularGrammar {
	public:

		using PatternList = std::deque<RegularPattern>;



		RegularGrammar() = default;

		RegularGrammar(RegularGrammar&&) = default;
		RegularGrammar& operator=(RegularGrammar&&) = default;

		RegularGrammar(const RegularGrammar&) = delete;
		RegularGrammar& operator=(const RegularGrammar&) = delete;



		/** @{ */
		/**
		 * @brief Add a new named pattern to the grammar.
		*/
		void addPattern(std::string_view name, regex::RegularExpr regex) {
			m_patterns.emplace_back(std::move(regex), name, uniquePatternId());
		}



		/**
		 * @brief List of all patterns defined by the grammar.
		*/
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