#ifndef PARSEC_FG_REGULAR_GRAMMAR_HEADER
#define PARSEC_FG_REGULAR_GRAMMAR_HEADER

#include "../core/NonCopyable.hpp"
#include "RegularPattern.hpp"

#include <unordered_map>
#include <span>
#include <vector>

namespace parsec::fg {
	
	/**
	 * @brief Describes a regular language using a set of string patterns.
	*/
	class RegularGrammar : private NonCopyable {
	public:

		/**
		 * @brief Add a new named pattern to the grammar.
		*/
		void addPattern(std::string name, regex::RegularExpr regex);



		/**
		 * @brief List of all patterns defined by the grammar.
		*/
		std::span<const RegularPattern> patterns() const {
			return m_patterns;
		}



	private:
		Id getUniquePatternId() const noexcept {
			return m_patterns.size();
		}

		std::vector<RegularPattern> m_patterns;
		std::unordered_map<std::string, Id> m_patternIndex;
	};

}

#endif