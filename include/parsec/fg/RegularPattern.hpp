#ifndef PARSEC_FG_REGULAR_PATTERN_HEADER
#define PARSEC_FG_REGULAR_PATTERN_HEADER

#include "../regex/nodes/CharAtom.hpp"
#include "../regex/RegularExpr.hpp"
#include "../core/NonCopyable.hpp"

#include <gsl/util>

#include <string>
#include <vector>

namespace parsec::fg {

	/**
	 * @brief Presents a regular expression as a sequence of its character positions.
	*/
	class RegularPattern : private NonCopyable {
	public:

		RegularPattern(std::string name, regex::RegularExpr regex, int id);

		RegularPattern()
			: RegularPattern("", regex::RegularExpr(), 0) {
		}



		/** @{ */
		/**
		 * @brief List of positions that follow the position given in at least one pattern-generated string.
		*/
		std::vector<gsl::index> followPos(gsl::index pos) const;



		/**
		 * @brief List of positions that start at least one pattern-generated string.
		*/
		std::vector<gsl::index> firstPos() const;
		
		
		
		/**
		 * @brief List of positions ending at least one pattern-generated string.
		*/
		std::vector<gsl::index> lastPos() const;



		/**
		 * @brief Get the character atom positioned at the specified index within the pattern.
		*/
		const regex::nodes::CharAtom* atomAt(gsl::index i) const {
			if(i < size()) {
				return m_atoms[i];
			}
			return nullptr;
		}



		/**
		 * @brief Number of character atoms in the pattern.
		*/
		std::size_t size() const {
			return m_atoms.size();
		}



		/**
		 * @brief Regular expression that defines the pattern.
		*/
		const regex::RegularExpr& regex() const {
			return m_regex;
		}



		/**
		 * @brief Unique string identifier for the pattern.
		*/
		const std::string& name() const {
			return m_name;
		}



		/**
		 * @brief Unique integer identifier for the pattern.
		*/
		int id() const {
			return m_id;
		}
		/** @} */



	private:
		regex::RegularExpr m_regex;
		std::vector<const regex::nodes::CharAtom*> m_atoms;
		
		std::string m_name;
		int m_id = {};
	};

}

#endif