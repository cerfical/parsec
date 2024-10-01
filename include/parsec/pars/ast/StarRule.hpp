#pragma once

#include "UnaryRule.hpp"

namespace parsec::pars {

    /**
     * @brief Denotes an application of the Kleene star operator to a rule.
     */
    class StarRule : public UnaryRule {
    public:

        using UnaryRule::UnaryRule;

        void accept(NodeVisitor& visitor) const override;
    };

}
