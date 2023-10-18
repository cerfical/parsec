#ifndef PARSEC_REGEX_CHAR_LITERAL_HEADER
#define PARSEC_REGEX_CHAR_LITERAL_HEADER

#include "ExprNode.hpp"

namespace parsec::regex {
	/**
	 * @brief Literal character regular expression.
	 */
	class CharLiteral : public ExprNode {
	public:
		/** @{ */
		/** @brief Construct a new node with from a value. */
		explicit CharLiteral(char value) noexcept
			: m_value(value)
		{ }
		/** @} */


		/** @{ */
		void traverseWith(NodeTraverser& traverser) const override;
		bool nullable() const noexcept override;
		/** @} */


		/** @{ */
		/** @brief Value of the character literal. */
		char value() const noexcept {
			return m_value;
		}


		/** @brief Finds the nodes taking the position after the node. */
		CharLiteralList followPos() const;
		/** @} */


	private:
		/** @{ */
		char m_value = '\0';
		/** @} */
	};
}

#endif