#pragma once

#include "BinaryRule.hpp"

namespace parsec::pars {

	/**
	 * @brief Concatenation of two rules.
	*/
	class ConcatRule : public BinaryRule {
	public:

		using BinaryRule::BinaryRule;

		void accept(NodeVisitor& visitor) const override;

	};

}
