#pragma once

#include "BinaryRuleNode.hpp"

namespace parsec::pars {

    /**
     * @brief Alternation of two rules.
     */
    class AlternRuleNode : public BinaryRuleNode {
    public:

        using BinaryRuleNode::BinaryRuleNode;


        /** @{ */
        void accept(NodeVisitor& visitor) const override;
        /** @} */
    };

}
