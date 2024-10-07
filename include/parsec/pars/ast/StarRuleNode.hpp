#pragma once

#include "UnaryRuleNode.hpp"

namespace parsec::pars {

    /**
     * @brief Denotes an application of the Kleene star operator to a rule.
     */
    class StarRuleNode : public UnaryRuleNode {
    public:

        using UnaryRuleNode::UnaryRuleNode;

        void accept(NodeVisitor& visitor) const override;
    };

}
