#pragma once

#include "../regex/ast/ExprNode.hpp"

#include "Symbol.hpp"

#include <memory>
#include <span>
#include <string_view>
#include <vector>

namespace parsec {

    /**
     * @brief Wraps a regular expression to make it easier to convert it to DFA.
     */
    class RegularExpr {
    public:

        /**
         * @brief Parse an expression from a string.
         */
        explicit RegularExpr(std::string_view regex = "");


        /**
         * @brief Construct an expression from its AST.
         */
        RegularExpr(regex::NodePtr rootNode);


        /**
         * @brief Compute the 'firstpos' set for the expression.
         */
        std::span<const int> firstPos() const {
            if(!state_) {
                return {};
            }

            return state_->firstPos;
        }


        /**
         * @brief Compute the 'followpos' set for a position.
         */
        std::span<const int> followPos(int pos) const {
            if(!state_) {
                return {};
            }

            if(const auto& fp = state_->followPos; pos >= 0 && pos < fp.size()) {
                return fp[pos];
            }
            return {};
        }


        /**
         * @brief Value of a symbol atom at position, if any.
         */
        const Symbol* valueAt(int pos) const {
            if(!state_) {
                return nullptr;
            }

            if(const auto& symbols = state_->symbols; pos >= 0 && pos < symbols.size()) {
                return &symbols[pos];
            }
            return nullptr;
        }


        /**
         * @brief Check if a position is the end position.
         */
        bool isEndPos(int pos) const {
            return valueAt(pos) == nullptr;
        }


    private:
        /**
         * @brief Represents the pattern as read-only data, computed once and shared between all pattern instances.
         */
        struct State {
            std::vector<Symbol> symbols;

            std::vector<std::vector<int>> followPos;
            std::vector<int> firstPos;
        };

        std::shared_ptr<const State> state_;
    };

}
