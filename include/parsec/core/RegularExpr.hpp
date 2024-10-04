#pragma once

#include "../regex/ast/ExprNode.hpp"
#include "../regex/make.hpp"

#include "Symbol.hpp"

#include <memory>
#include <ostream>
#include <string_view>
#include <utility>
#include <vector>

namespace parsec {

    /**
     * @brief Presents a regular expression as a linear sequence of symbols.
     */
    class RegularExpr {
    public:

        friend RegularExpr operator+(const RegularExpr& lhs, const RegularExpr& rhs) {
            return regex::concat(lhs, rhs);
        }

        friend RegularExpr operator|(const RegularExpr& lhs, const RegularExpr& rhs) {
            return regex::altern(lhs, rhs);
        }


        friend RegularExpr& operator+=(RegularExpr& lhs, const RegularExpr& rhs) {
            return lhs = lhs + rhs;
        }

        friend RegularExpr& operator|=(RegularExpr& lhs, const RegularExpr& rhs) {
            return lhs = lhs | rhs;
        }


        using PosList = std::vector<int>;


        RegularExpr(const RegularExpr&) noexcept = default;
        RegularExpr& operator=(const RegularExpr&) noexcept = default;

        RegularExpr(RegularExpr&&) noexcept = default;
        RegularExpr& operator=(RegularExpr&&) noexcept = default;

        ~RegularExpr() = default;


        /** @{ */
        /**
         * @brief Construct an empty regular expression.
         */
        RegularExpr() noexcept = default;


        /**
         * @brief Parse a regular expression from a string.
         */
        explicit RegularExpr(std::string_view regex);


        /**
         * @brief Construct a regular expression directly from its AST.
         */
        RegularExpr(regex::NodePtr rootNode) noexcept
            : rootNode_(std::move(rootNode)) {}
        /** @} */


        /** @{ */
        /**
         * @brief Get the 'firstpos' set for the expression.
         */
        const PosList& firstPos() const;


        /**
         * @brief Get the 'followpos' set for a position in the expression.
         */
        const PosList& followPos(int pos) const;


        /**
         * @brief Get a symbol for a position in the expression.
         */
        const Symbol& posValue(int pos) const;


        /**
         * @brief Check if a position is the end position for the expression.
         */
        bool isEndPos(int pos) const {
            return posValue(pos).isEmpty();
        }


        /**
         * @brief Convert the regular expression to its AST equivalent.
         */
        operator regex::NodePtr() const noexcept {
            return rootNode_;
        }


        /**
         * @brief Check if the regular expression is non-empty.
         */
        explicit operator bool() const noexcept {
            return !isEmpty();
        }


        /**
         * @brief Check if the regular expression is empty.
         */
        bool isEmpty() const noexcept {
            return rootNode_ == nullptr;
        }
        /** @} */


    private:
        static inline PosList emptyPosList;
        static inline Symbol emptyPosValue;

        class ComputeCache;
        ComputeCache* computeCache() const;

        mutable std::shared_ptr<ComputeCache> computeCache_;
        regex::NodePtr rootNode_;
    };

    std::ostream& operator<<(std::ostream& out, const RegularExpr& expr);

}
