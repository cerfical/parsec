#pragma once

#include "../Token.hpp"

#include "Node.hpp"

namespace parsec::pars {

    /**
     * @brief Represents a rule with a name.
     */
    class NamedRule : public Node {
    public:

        NamedRule(Token name, NodePtr rule) noexcept
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
