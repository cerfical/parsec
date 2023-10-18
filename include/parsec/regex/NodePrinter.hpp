#ifndef PARSEC_REGEX_NODE_PRINTER_HEADER
#define PARSEC_REGEX_NODE_PRINTER_HEADER

#include "ExprNode.hpp"
#include "NodeTraverser.hpp"

#include <iostream>

namespace parsec::regex {
	/**
	 * @brief Prints regular expressions.
	 */
	class NodePrinter : public NodeTraverser {
	public:
		/** @{ */
		/** @brief Construct a new printer that prints to a @c std::ostream. */
		explicit NodePrinter(std::ostream& out = std::cout) noexcept
			: m_out(&out)
		{ }
		/** @} */


		/** @{ */
		NodePrinter(NodePrinter&&) = default;
		NodePrinter& operator=(NodePrinter&&) = default;
		/** @} */


		/** @{ */
		NodePrinter(const NodePrinter&) = delete;
		NodePrinter& operator=(const NodePrinter&) = delete;
		/** @} */


		/** @{ */
		/** @brief Prints out a regular expression recursively. */
		void print(const ExprNode& n) {
			n.traverseWith(*this);
		}
		/** @} */


	private:
		/** @{ */
		void visitNode(const CharLiteral& n) override;
		void visitNode(const NilExpr& n) override;

		void visitNode(const OptionalExpr& n) override;
		void visitNode(const PlusExpr& n) override;
		void visitNode(const StarExpr& n) override;

		void visitNode(const AlternExpr& n) override;
		void visitNode(const ConcatExpr& n) override;
		/** @} */


		/** @{ */
		std::ostream* m_out = nullptr;
		/** @} */
	};
}

#endif