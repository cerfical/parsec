#ifndef PARSEC_FG_NODE_PRINTER_HEADER
#define PARSEC_FG_NODE_PRINTER_HEADER

#include "SyntaxNode.hpp"
#include "NodeTraverser.hpp"

#include <iostream>

namespace parsec::fg {
	/**
	 * @brief Prints syntax trees.
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
		/** @brief Prints out a syntax tree recursively. */
		void print(const SyntaxNode& n) {
			n.traverseWith(*this);
		}
		/** @} */


	private:
		/** @{ */
		void visitNode(const RuleRef& n) override;
		
		void visitNode(const AlternNode& n) override;
		void visitNode(const ConcatNode& n) override;
		/** @} */


		/** @{ */
		std::ostream* m_out = nullptr;
		/** @} */
	};
}

#endif