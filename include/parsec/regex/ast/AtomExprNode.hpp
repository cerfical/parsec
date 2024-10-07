#pragma once

#include "../../core/Symbol.hpp"
#include "ExprNode.hpp"

namespace parsec::regex {

    /**
     * @brief Atomic expression consisting of a single symbol.
     */
    class AtomExprNode : public ExprNode {
    public:

        explicit AtomExprNode(Symbol value)
            : value_(std::move(value)) {}


        void accept(NodeVisitor& visitor) const override;

        bool isNullable() const noexcept override {
            return value_.isEmpty();
        }

        int atomCount() const noexcept override {
            return value_ ? 1 : 0;
        }


        /**
         * @brief Value of the symbol.
         */
        const Symbol& value() const noexcept {
            return value_;
        }


    private:
        Symbol value_;
    };

}
