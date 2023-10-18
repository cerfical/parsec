#ifndef PARSEC_FG_GRAMMAR_NODE_HEADER
#define PARSEC_FG_GRAMMAR_NODE_HEADER

#include <iostream>

namespace parsec::fg {
	/**
	 * @brief Polymorphic entry point to the tree representation of a @ref GrammarRule "grammar rule".
	 */
	class GrammarNode {
	public:
		/** @{ */
		/** @brief Output stream operator for a node. */
		friend std::ostream& operator<<(std::ostream& out, const GrammarNode& n) {
			n.print(out);
			return out;
		}
		/** @} */


		/** @{ */
		/** @brief Default initialization of the node. */
		GrammarNode() = default;


		/** @brief Destroy the node. */
		virtual ~GrammarNode() = default;
		/** @} */


		/** @{ */
		GrammarNode(const GrammarNode&) = delete;
		GrammarNode& operator=(const GrammarNode&) = delete;
		/** @} */


		/** @{ */
		GrammarNode(GrammarNode&&) = delete;
		GrammarNode& operator=(GrammarNode&&) = delete;
		/** @} */


		/** @{ */
		/** @brief Prints out the node to a @c std::ostream. */
		virtual void print(std::ostream& out = std::cout) const = 0;
		/** @} */
	};
}

#endif