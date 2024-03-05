#ifndef PARSEC_FG_REGULAR_PATTERN_HEADER
#define PARSEC_FG_REGULAR_PATTERN_HEADER

#include "../regex/nodes/CharAtom.hpp"
#include "../regex/RegularExpr.hpp"
#include "../core/NonCopyable.hpp"
#include "../core/typedefs.hpp"

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

		RegularPattern()
			: RegularPattern("", regex::RegularExpr(), 0) {
		}

		RegularPattern(std::string name, regex::RegularExpr regex, Id id);



		/** @{ */
		/**
		 * @brief Add an additional regular expresion to the pattern.
		*/
		void addExpr(regex::RegularExpr regex);



		/**
		 * @brief List of positions that start at least one pattern-generated string.
		*/
		IndexList firstPos() const;



		/**
		 * @brief List of positions that follow the position given in at least one pattern-generated string.
		*/
		IndexList followPos(Index pos) const;
		
		
		
		/**
		 * @brief Character positioned at the specified index within the pattern, if any.
		*/
		std::optional<char> charAt(Index pos) const;



		/**
		 * @brief Unique string identifier for the pattern.
		*/
		const std::string& name() const {
			return m_name;
		}



		/**
		 * @brief Unique integer identifier for the pattern.
		*/
		Id id() const {
			return m_id;
		}
		/** @} */



	private:
		class IsNullable;
		class ComputeFirstPos;
		class ComputeFollowPos;
		class CollectAtomInfo;


		Index atomId(const regex::nodes::CharAtom& atom) const noexcept {
			return m_atomIndex.find(&atom)->second;
		}

		regex::RegularExpr m_regex;
		std::unordered_map<const regex::nodes::CharAtom*, Index> m_atomIndex;
		std::vector<const regex::nodes::CharAtom*> m_atoms;
		
		std::string m_name;
		Id m_id = {};
	};

}

#endif