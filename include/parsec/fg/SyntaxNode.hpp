#ifndef PARSEC_FG_SYNTAX_NODE_HEADER
#define PARSEC_FG_SYNTAX_NODE_HEADER

#include <ostream>

namespace parsec::fg {
	class NodeTraverser;



	/**
	 * @brief Tree representation of a syntactic construct for a language.
	 */
	class SyntaxNode {
	public:
		/** @{ */
		/** @brief Output stream operator for syntax nodes. */
		friend std::ostream& operator<<(std::ostream& out, const SyntaxNode& n);
		/** @} */


		/** @{ */
		SyntaxNode() = default;
		virtual ~SyntaxNode() = default;
		/** @} */


		/** @{ */
		SyntaxNode(const SyntaxNode&) = delete;
		SyntaxNode& operator=(const SyntaxNode&) = delete;
		/** @} */


		/** @{ */
		SyntaxNode(SyntaxNode&&) = delete;
		SyntaxNode& operator=(SyntaxNode&&) = delete;
		/** @} */


		/** @{ */
		/** @brief Traverse the syntax tree in a type-safe manner. */
		virtual void traverseWith(NodeTraverser& traverser) const = 0;
		/** @} */
	};
}

#endif