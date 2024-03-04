#ifndef PARSEC_FG_REGULAR_PATTERN_HEADER
#define PARSEC_FG_REGULAR_PATTERN_HEADER

#include "../regex/nodes/CharAtom.hpp"
#include "../regex/RegularExpr.hpp"
#include "../core/NonCopyable.hpp"

#include <unordered_map>
#include <optional>
#include <string>
#include <vector>

namespace parsec::fg {

	/**
	 * @brief Presents a regular expression as a sequence of its character positions.
	*/
	class RegularPattern : private NonCopyable {
	public:

		friend std::ostream& operator<<(std::ostream& out, const RegularPattern& pattern) {
			out << *pattern.m_regex.rootNode();
			return out;
		}



		RegularPattern()
			: RegularPattern("", regex::RegularExpr(), 0) {
		}

		RegularPattern(std::string name, regex::RegularExpr regex, int id);



		/** @{ */
		/**
		 * @brief List of positions that start at least one pattern-generated string.
		*/
		std::vector<std::size_t> firstPos() const;



		/**
		 * @brief List of positions that follow the position given in at least one pattern-generated string.
		*/
		std::vector<std::size_t> followPos(std::size_t i) const;
		
		
		
		/**
		 * @brief Character positioned at the specified index within the pattern, if any.
		*/
		std::optional<char> charAt(std::size_t i) const;



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
		class IsNullable;
		class ComputeFirstPos;
		class ComputeFollowPos;
		class CollectAtomInfo;


		std::size_t atomId(const regex::nodes::CharAtom& atom) const noexcept {
			return m_atomIndex.find(&atom)->second;
		}

		regex::RegularExpr m_regex;
		std::unordered_map<const regex::nodes::CharAtom*, std::size_t> m_atomIndex;
		std::vector<const regex::nodes::CharAtom*> m_atoms;
		
		std::string m_name;
		int m_id = {};
	};

}

#endif