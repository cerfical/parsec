#ifndef PARSEC_FSM_REGULAR_GRAMMAR_HEADER
#define PARSEC_FSM_REGULAR_GRAMMAR_HEADER

#include "../core/NonCopyable.hpp"
#include "RegularPattern.hpp"

#include <gsl/narrow>
#include <span>
#include <vector>

namespace parsec::fsm {
	
	/**
	 * @brief Describes a regular language using a set of string patterns.
	*/
	class RegularGrammar : private NonCopyable {
	public:

		/**
		 * @brief Add a new named pattern to the grammar.
		*/
		void addPattern(std::string name, regex::RegularExpr regex) {
			m_patterns.emplace_back(std::move(name), std::move(regex), getUniquePatternId());
		}


		/**
		 * @brief List of all patterns defined by the grammar.
		*/
		std::span<const RegularPattern> patterns() const {
			return m_patterns;
		}


	private:
		int getUniquePatternId() const {
			return gsl::narrow_cast<int>(m_patterns.size());
		}

		std::vector<RegularPattern> m_patterns;
	};

}

#endif