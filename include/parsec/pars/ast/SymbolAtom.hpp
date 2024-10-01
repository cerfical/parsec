#pragma once

#include "../Token.hpp"

#include "Node.hpp"

namespace parsec::pars {

    /**
     * @brief Trivial rule consisting of a single symbolic name.
     */
    class SymbolAtom : public Node {
    public:

        explicit SymbolAtom(Token value) noexcept
            : value_(std::move(value)) {}


        void accept(NodeVisitor& visitor) const override;


        /**
         * @brief Value of the symbolic name.
         */
        const Token& value() const noexcept {
            return value_;
        }


    private:
        Token value_;
    };

}
