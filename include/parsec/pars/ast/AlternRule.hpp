#pragma once

#include "BinaryRule.hpp"

namespace parsec::pars {

    /**
     * @brief Alternation of two rules.
     */
    class AlternRule : public BinaryRule {
    public:

        using BinaryRule::BinaryRule;

        void accept(NodeVisitor& visitor) const override;
    };

}
