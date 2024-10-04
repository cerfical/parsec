#pragma once

#include "UnaryExpr.hpp"

namespace parsec::regex {

    class StarClosure : public UnaryExpr {
    public:

        using UnaryExpr::UnaryExpr;


        void accept(NodeVisitor& visitor) const override;

        bool isNullable() const noexcept override {
            return true;
        }
    };

}
