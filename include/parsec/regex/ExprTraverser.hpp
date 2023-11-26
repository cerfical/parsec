#ifndef PARSEC_REGEX_EXPR_TRAVERSER_HEADER
#define PARSEC_REGEX_EXPR_TRAVERSER_HEADER

#include "NodeVisitor.hpp"
#include "ExprNode.hpp"

namespace parsec::regex {
	/**
	 * @brief Traverses regular expression atom by atom.
	 */
	class ExprTraverser : NodeVisitor {
	protected:
		~ExprTraverser() = default;


		void traverse(const ExprNode& n) {
			n.acceptVisitor(*this);
		}


		/** @{ */
		void visit(const CharAtom& n) override;
		void visit(const NilExpr& n) override;

		void visit(const PlusExpr& n) override;
		void visit(const StarExpr& n) override;
		void visit(const OptionalExpr& n) override;
		
		void visit(const AlternExpr& n) override;
		void visit(const ConcatExpr& n) override;
		/** @} */
	};
}

#endif