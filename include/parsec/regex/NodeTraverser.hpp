#ifndef PARSEC_REGEX_NODE_TRAVERSER_HEADER
#define PARSEC_REGEX_NODE_TRAVERSER_HEADER

namespace parsec::regex {
	class CharLiteral;
	class NilExpr;
	
	class PlusExpr;
	class StarExpr;
	class OptionalExpr;

	class AlternExpr;
	class ConcatExpr;



	/**
	 * @brief Provides a mechanism for traversing regular expressions using the visitor pattern.
	 */
	class NodeTraverser {
	public:
		/** @{ */
		/** @brief Called for a CharLiteral node. */
		virtual void visitNode(const CharLiteral& n) = 0;


		/** @brief Called for a NilExpr node. */
		virtual void visitNode(const NilExpr& n) = 0;

		
		/** @brief Called for a PlusExpr node. */
		virtual void visitNode(const PlusExpr& n) = 0;


		/** @brief Called for a StarExpr node. */
		virtual void visitNode(const StarExpr& n) = 0;


		/** @brief Called for a OptionalExpr node. */
		virtual void visitNode(const OptionalExpr& n) = 0;


		/** @brief Called for a AlternExpr node. */
		virtual void visitNode(const AlternExpr& n) = 0;


		/** @brief Called for a ConcatExpr node. */
		virtual void visitNode(const ConcatExpr& n) = 0;
		/** @} */


	protected:
		/** @{ */
		~NodeTraverser() = default;
		/** @} */
	};
}

#endif