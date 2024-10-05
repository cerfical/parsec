#pragma once

#include "UnaryExpr.hpp"

namespace parsec::regex {

    /**
     * @brief Denotes an 'optional' expresion.
     */
    class OptionalExpr : public UnaryExpr {
    public:

        using UnaryExpr::UnaryExpr;


        void accept(NodeVisitor& visitor) const override;

        bool isNullable() const noexcept override {
            return true;
        }
    };

}
