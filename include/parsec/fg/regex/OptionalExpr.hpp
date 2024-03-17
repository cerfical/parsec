#ifndef PARSEC_FG_REGEX_OPTIONAL_EXPR_HEADER
#define PARSEC_FG_REGEX_OPTIONAL_EXPR_HEADER

#include "UnaryExpr.hpp"

namespace parsec::fg::regex {
	
	class OptionalExpr : public UnaryExpr {
	public:
		using UnaryExpr::UnaryExpr;

		void accept(NodeVisitor& visitor) const override;

		bool isNullable() const noexcept override {
			return true;
		}
	};

}

#endif