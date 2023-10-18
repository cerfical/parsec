#ifndef PARSEC_FG_NODE_TRAVERSER_HEADER
#define PARSEC_FG_NODE_TRAVERSER_HEADER

namespace parsec::fg {
	class RuleRef;
	class AlternNode;
	class ConcatNode;



	/**
	 * @brief Provides a mechanism for traversing syntax trees using the visitor pattern.
	 */
	class NodeTraverser {
	public:
		/** @{ */
		/** @brief Called for a RuleRef node. */
		virtual void visitNode(const RuleRef& n) = 0;


		/** @brief Called for a AlternNode node. */
		virtual void visitNode(const AlternNode& n) = 0;


		/** @brief Called for a ConcatNode node. */
		virtual void visitNode(const ConcatNode& n) = 0;
		/** @} */


	protected:
		/** @{ */
		~NodeTraverser() = default;
		/** @} */
	};
}

#endif