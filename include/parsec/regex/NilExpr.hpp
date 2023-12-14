#ifndef PARSEC_REGEX_NIL_EXPR_HEADER
#define PARSEC_REGEX_NIL_EXPR_HEADER

#include "ExprNode.hpp"

namespace parsec::regex {
	
	/**
	 * @brief Empty regular expression.
	 */
	class NilExpr : public ExprNode {
	public:
		
		NilExpr() = default;

		void acceptVisitor(NodeVisitor& visitor) const override;

		int atomCount() const noexcept override {
			return 0;
		}

	private:
		void rebaseAtomIndices(int) noexcept override
		{ }
	};

}

#endif