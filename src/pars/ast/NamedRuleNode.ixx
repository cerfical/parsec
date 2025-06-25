module;

#include <utility>

export module parsec.pars:ast.NamedRuleNode;

import :ast.Node;
import :Token;

namespace parsec::pars {

    /**
     * @brief Represents a rule with a name.
     */
    export class NamedRuleNode : public Node {
    public:

        NamedRuleNode(Token name, NodePtr rule)
            : name_(std::move(name)), rule_(std::move(rule)) {}

        void accept(NodeVisitor& visitor) const override;


        /**
         * @brief Name of the rule.
         */
        const Token& name() const noexcept {
            return name_;
        }


        /**
         * @brief Body of the rule.
         */
        const Node* rule() const noexcept {
            return rule_.get();
        }


    private:
        Token name_;
        NodePtr rule_;
    };

}
