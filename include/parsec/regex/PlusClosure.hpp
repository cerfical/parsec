#ifndef PARSEC_REGEX_PLUS_CLOSURE_HEADER
#define PARSEC_REGEX_PLUS_CLOSURE_HEADER

#include "UnaryExpr.hpp"

namespace parsec::regex {

	class PlusClosure : public UnaryExpr {
	public:

		using UnaryExpr::UnaryExpr;


		void accept(NodeVisitor& visitor) const override;

		bool isNullable() const noexcept override {
			return inner()->isNullable();
		}

	};

}

#endif