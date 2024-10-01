#pragma once

#include "UnaryRule.hpp"

namespace parsec::pars {

    /**
     * @brief Denotes an application of the Kleene plus operator to a rule.
     */
    class PlusRule : public UnaryRule {
    public:

        using UnaryRule::UnaryRule;

        void accept(NodeVisitor& visitor) const override;
    };

}
