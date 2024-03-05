#ifndef PARSEC_REGEX_REGULAR_EXPR_HEADER
#define PARSEC_REGEX_REGULAR_EXPR_HEADER

#include "../core/NonCopyable.hpp"
#include "nodes/ExprNode.hpp"

#include <string_view>

namespace parsec::regex {
	
	/**
	 * @brief Presents a regular expression as a tree hierarchy, with an internal node for each operation and a leaf node for each character atom.
	*/
	class RegularExpr : private NonCopyable {
	public:

		RegularExpr()
			: RegularExpr("") {
		}

		/**
		 * @brief Create a new regular expression from a string pattern.
		 * @throws ParseError
		*/
		explicit RegularExpr(std::string_view regex);



		/** @{ */
		/**
		 * @brief Combine two expressions with the alternation operator.
		*/
		void altern(RegularExpr other);



		/**
		 * @brief Combine two expressions with the concatenation operator.
		*/
		void concat(RegularExpr other);



		/**
		 * @brief Make the expression to repeat with the Kleense plus operator.
		*/
		void repeat();



		/**
		 * @brief A node for the top-level subexpression.
		*/
		const nodes::ExprNode* rootNode() const {
			return m_rootNode.get();
		}



		/**
		 * @brief Get a string representation of the expression.
		*/
		std::string toStr() const;
		/** @} */



	private:
		nodes::ExprPtr m_rootNode;
	};



	inline bool operator==(const RegularExpr& rhs, const RegularExpr& lhs) noexcept {
		return rhs.rootNode()->isEqualTo(*rhs.rootNode());
	}

	inline std::ostream& operator<<(std::ostream& out, const RegularExpr& expr) {
		out << expr.toStr();
		return out;
	}

}

#endif