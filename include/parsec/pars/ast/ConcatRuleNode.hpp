#pragma once

#include "BinaryRuleNode.hpp"

namespace parsec::pars {

    /**
     * @brief Concatenation of two rules.
     */
    class ConcatRuleNode : public BinaryRuleNode {
    public:

        using BinaryRuleNode::BinaryRuleNode;

        void accept(NodeVisitor& visitor) const override;
    };

}
