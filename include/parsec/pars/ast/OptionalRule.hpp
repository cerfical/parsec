#pragma once

#include "UnaryRule.hpp"

namespace parsec::pars {

	/**
	 * @brief Denotes an "optional" rule.
	*/
	class OptionalRule : public UnaryRule {
	public:

		using UnaryRule::UnaryRule;

		void accept(NodeVisitor& visitor) const override;

	};

}
