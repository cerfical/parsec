#pragma once

#include "UnaryRuleNode.hpp"

namespace parsec::pars {

    /**
     * @brief Denotes an 'optional' rule.
     */
    class OptionalRuleNode : public UnaryRuleNode {
    public:

        using UnaryRuleNode::UnaryRuleNode;


        /** @{ */
        void accept(NodeVisitor& visitor) const override;
        /** @} */
    };

}
