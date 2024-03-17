#ifndef PARSEC_FG_REGEX_STAR_CLOSURE_HEADER
#define PARSEC_FG_REGEX_STAR_CLOSURE_HEADER

#include "UnaryExpr.hpp"

namespace parsec::fg::regex {
	
	class StarClosure : public UnaryExpr {
	public:
		using UnaryExpr::UnaryExpr;

		void accept(NodeVisitor& visitor) const override;

		bool isNullable() const noexcept override {
			return true;
		}
	};

}

#endif