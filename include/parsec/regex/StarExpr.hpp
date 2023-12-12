#ifndef PARSEC_REGEX_STAR_EXPR_HEADER
#define PARSEC_REGEX_STAR_EXPR_HEADER

#include "UnaryExpr.hpp"

namespace parsec::regex {
	
	/**
	 * @brief Marks an inner expression as being repeated zero or more times.
	 */
	class StarExpr : public UnaryExpr {
	public:
		
		explicit StarExpr(ExprPtr inner) noexcept
			: UnaryExpr(std::move(inner))
		{ }


		StarExpr(StarExpr&&) = default;
		StarExpr& operator=(StarExpr&&) = default;


		/** @{ */
		void acceptVisitor(NodeVisitor& visitor) const override;
		/** @} */

	};

}

#endif