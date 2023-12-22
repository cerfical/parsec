#ifndef PARSEC_REGEX_NODES_OPTIONAL_EXPR_HEADER
#define PARSEC_REGEX_NODES_OPTIONAL_EXPR_HEADER

#include "UnaryExpr.hpp"

namespace parsec::regex::nodes {
	
	/**
	 * @brief Marks an inner expression as 'optional', meaning it is either present or not.
	 */
	class OptionalExpr : public UnaryExpr {
	public:
		
		explicit OptionalExpr(ExprPtr inner) noexcept
			: UnaryExpr(std::move(inner))
		{ }

		void acceptVisitor(NodeVisitor& visitor) const override;

	};

}

#endif