#ifndef PARSEC_REGEX_REGULAR_EXPR_HEADER
#define PARSEC_REGEX_REGULAR_EXPR_HEADER

#include "nodes/ExprNode.hpp"

#include <string_view>
#include <string>

namespace parsec::regex {
	
	/**
	 * @brief Presents a regular expression as a tree hierarchy, with an internal node for each operation and a leaf node for each character atom.
	*/
	class RegularExpr {
	public:

		/**
		 * @brief Create a new regular expression from a string pattern.
		 * @throws ParseError
		*/
		explicit RegularExpr(std::string_view regex= "");



		/** @{ */
		RegularExpr(const RegularExpr&) = delete;
		RegularExpr& operator=(const RegularExpr&) = delete;

		RegularExpr(RegularExpr&&) = default;
		RegularExpr& operator=(RegularExpr&&) = default;
		/** @} */



		/** @{ */
		/**
		 * @brief Get a node for the top-level subexpression.
		*/
		const nodes::ExprNode* rootNode() const noexcept {
			return m_rootNode.get();
		}



		/**
		 * @brief Get a string from which the expression was compiled.
		*/
		std::string_view str() const noexcept {
			return m_str;
		}
		/** @} */



	private:
		nodes::ExprPtr m_rootNode;
		std::string m_str;
	};

}

#endif