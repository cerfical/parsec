#pragma once

#include "RegularExpr.hpp"
#include "Symbol.hpp"

#include <utility>

namespace parsec {

    /**
     * @brief Defines a relationship between symbols to form other symbols.
     */
    class SymbolRule {
    public:

        SymbolRule(const SymbolRule&) noexcept = default;
        SymbolRule& operator=(const SymbolRule&) noexcept = default;

        SymbolRule(SymbolRule&&) noexcept = default;
        SymbolRule& operator=(SymbolRule&&) noexcept = default;

        ~SymbolRule() = default;


        /** @{ */
        /**
         * @brief Construct an empty rule.
         */
        SymbolRule() noexcept = default;


        /**
         * @brief Construct a rule with a head and a body.
         */
        SymbolRule(Symbol head, RegularExpr body) noexcept
            : head_(std::move(head)), body_(std::move(body)) {}
        /** @} */


        /** @{ */
        /**
         * @brief Check if the rule has a non-empty body.
         */
        explicit operator bool() const noexcept {
            return !isEmpty();
        }


        /**
         * @brief Check if the rule has an empty body.
         */
        bool isEmpty() const noexcept {
            return body().isEmpty();
        }


        /**
         * @brief The combination of symbols to yield a new symbol instance.
         */
        const RegularExpr& body() const noexcept {
            return body_;
        }


        /**
         * @brief The symbol yielded by the body.
         */
        const Symbol& head() const noexcept {
            return head_;
        }
        /** @} */


    private:
        Symbol head_;
        RegularExpr body_;
    };

}
