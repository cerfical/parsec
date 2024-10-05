#pragma once

#include "UnaryExpr.hpp"

namespace parsec::regex {

    /**
     * @brief Denotes an application of the Kleene plus operator to an expression.
     */
    class PlusClosure : public UnaryExpr {
    public:

        using UnaryExpr::UnaryExpr;


        void accept(NodeVisitor& visitor) const override;

        bool isNullable() const noexcept override {
            return inner()->isNullable();
        }
    };

}
