#ifndef PARSEC_REGEX_CONCAT_EXPR_HEADER
#define PARSEC_REGEX_CONCAT_EXPR_HEADER

#include "BinaryExpr.hpp"

namespace parsec::regex {
	
	class ConcatExpr : public BinaryExpr {
	public:

		using BinaryExpr::BinaryExpr;


		void accept(NodeVisitor& visitor) const override;

		bool isNullable() const noexcept override {
			return left()->isNullable() && right()->isNullable();
		}

	};

}

#endif