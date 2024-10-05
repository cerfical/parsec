#pragma once

#include "UnaryRuleNode.hpp"

namespace parsec::pars {

    /**
     * @brief Denotes an application of the Kleene plus operator to a rule.
     */
    class PlusRuleNode : public UnaryRuleNode {
    public:

        using UnaryRuleNode::UnaryRuleNode;


        /** @{ */
        void accept(NodeVisitor& visitor) const override;
        /** @} */
    };

}
