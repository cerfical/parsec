#ifndef PARSEC_REGEX_ROOT_HEADER
#define PARSEC_REGEX_ROOT_HEADER

#include "../utils/utils.hpp"

#include "Parser.hpp"
#include "Traverser.hpp"
#include "Printer.hpp"

#include "nodes.hpp"

#include <gsl/gsl>

/** @brief Root namespace for regular expression classes and functions. */
namespace parsec::regex {
	/**
	 * @brief Compiled regular expression.
	 */
	class RegEx {
	public:
		/** @{ */
		/** @brief Print out a regular expression to a @c std::ostream. */
		friend std::ostream& operator<<(std::ostream& out, const RegEx& regex);
		/** @} */


		/** @{ */
		/** @brief Construct an empty regular expression. */
		RegEx() : RegEx("")
		{ }

		/** @brief Construct a regular expression from the root node. */
		explicit RegEx(std::unique_ptr<ExprNode> root);

		/** @brief Compile a new regular expression from an arbitrary character sequence. */
		RegEx(std::string_view regex);

		/** @brief Compile a new regular expression from a string. */
		RegEx(const std::string& regex)
			: RegEx(std::string_view(regex))
		{ }

		/** @brief Compile a new regular expression from a C-style string. */
		RegEx(gsl::czstring regex)
			: RegEx(std::string_view(regex))
		{ }
		/** @} */


		/** @{ */
		/** @brief Root node of the regular expression. */
		const ExprNode* rootNode() const noexcept {
			return m_rootNode.get();
		}

		/** @brief Special position in the regular expression that identifies its end. */
		const CharLiteral* endMarker() const noexcept {
			return static_cast<const CharLiteral*>(
				static_cast<const BinaryExpr*>(rootNode())->rightSubexpr()
			);
		}

		/** @brief Retrieve the string representation of the regular expression. */
		std::string toStr() const;
		/** @} */

	private:
		std::unique_ptr<ExprNode> m_rootNode;
	};
}

#endif