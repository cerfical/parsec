#pragma once

#include "BinaryExpr.hpp"

namespace parsec::regex {

	class AlternExpr : public BinaryExpr {
	public:

		using BinaryExpr::BinaryExpr;


		void accept(NodeVisitor& visitor) const override;

		bool isNullable() const noexcept override {
			return left()->isNullable() || right()->isNullable();
		}

	};

}
