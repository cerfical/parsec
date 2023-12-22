#ifndef PARSEC_BNF_REGULAR_PATTERN_HEADER
#define PARSEC_BNF_REGULAR_PATTERN_HEADER

#include "../regex/nodes/CharAtom.hpp"
#include "../regex/RegularExpr.hpp"

#include <gsl/util>

#include <string>
#include <string_view>
#include <vector>

namespace parsec::bnf {

	/**
	 * @brief Views a regular expression as a sequence of positions, each assigned a corresponding character atom.
	*/
	class RegularPattern {
	public:

		RegularPattern(regex::RegularExpr regex, std::string_view name, int id);



		/** @{ */
		RegularPattern(const RegularPattern&) = delete;
		RegularPattern& operator=(const RegularPattern&) = delete;

		RegularPattern(RegularPattern&&) = default;
		RegularPattern& operator=(RegularPattern&&) = default;
		/** @} */



		/** @{ */
		/**
		 * @brief Get a list of positions that follow the position given in at least one string generated from the pattern.
		*/
		std::vector<gsl::index> followPos(gsl::index pos) const;



		/**
		 * @brief Get a list of positions that start at least one string generated from the pattern.
		*/
		std::vector<gsl::index> firstPos() const;
		
		
		
		/**
		 * @brief Get a list of positions ending at least one string generated from the pattern.
		*/
		std::vector<gsl::index> lastPos() const;



		/**
		 * @brief Get the character atom positioned at the specified index within the pattern.
		*/
		const regex::nodes::CharAtom* atomAt(gsl::index i) const noexcept {
			if(i < size()) {
				return m_atoms[i];
			}
			return nullptr;
		}



		/**
		 * @brief Get the number of character atoms in the underlying regular expression.
		*/
		std::size_t size() const noexcept {
			return m_atoms.size();
		}



		/**
		 * @brief Get the underlying regular expression.
		*/
		const regex::RegularExpr& expr() const noexcept {
			return m_expr;
		}



		/**
		 * @brief Get the string identifier for the pattern.
		*/
		std::string_view name() const noexcept {
			return m_name;
		}



		/**
		 * @brief Get the unique integer identifier for the pattern.
		*/
		int id() const noexcept {
			return m_id;
		}
		/** @} */



	private:
		regex::RegularExpr m_expr;
		std::vector<const regex::nodes::CharAtom*> m_atoms;
		
		std::string m_name;
		int m_id = {};
	};

}

#endif