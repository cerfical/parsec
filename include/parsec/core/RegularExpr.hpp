#pragma once

#include "../regex/ast/ConcatExprNode.hpp"
#include "../regex/ast/ExprNode.hpp"
#include "../regex/make.hpp"

#include "Symbol.hpp"

#include <memory>
#include <span>
#include <string_view>
#include <utility>
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
        RegularExpr(regex::NodePtr rootNode)
            : rootNode_(std::move(rootNode)) {}


        /**
         * @brief Compute the 'firstpos' set for the expression.
         */
        std::span<const int> firstPos() const {
            return memo()->firstPos;
        }


        /**
         * @brief Compute the 'followpos' set for a position.
         */
        std::span<const int> followPos(int pos) const {
            if(const auto& fp = memo()->followPos; pos >= 0 && pos < fp.size()) {
                return fp[pos];
            }
            return {};
        }


        /**
         * @brief Value of a symbol atom at position, if any.
         */
        const Symbol* valueAt(int pos) const {
            if(const auto& atoms = memo()->atoms; pos >= 0 && pos < atoms.size()) {
                return &atoms[pos]->value();
            }
            return nullptr;
        }


        /**
         * @brief Check if a position is the end position.
         */
        bool isEndPos(int pos) const {
            return valueAt(pos) == nullptr;
        }


        /**
         * @brief Convert the expression to its AST.
         */
        operator regex::NodePtr() const noexcept {
            return rootNode();
        }


        /**
         * @brief AST representation of the expression.
         */
        regex::NodePtr rootNode() const noexcept {
            return rootNode_;
        }


    private:
        struct Memo {

            Memo(regex::NodePtr rootNode);


            regex::ConcatExprNode regex;

            std::vector<const regex::AtomExprNode*> atoms;
            std::vector<int> firstPos;
            std::vector<std::vector<int>> followPos;
        };


        Memo* memo() const {
            if(!memo_) {
                memo_ = std::make_shared<Memo>(rootNode_);
            }
            return memo_.get();
        }

        mutable std::shared_ptr<Memo> memo_;
        regex::NodePtr rootNode_;
    };


    /**
     * @related RegularExpr
     * @brief Create a concatenation expression out of two expressions.
     */
    inline RegularExpr operator+(const RegularExpr& lhs, const RegularExpr& rhs) {
        return regex::concat(lhs, rhs);
    }


    /**
     * @related RegularExpr
     * @brief Create a concatenation expression out of two expressions in place.
     */
    inline RegularExpr& operator+=(RegularExpr& lhs, const RegularExpr& rhs) {
        return lhs = lhs + rhs;
    }


    /**
     * @related RegularExpr
     * @brief Create an alternation expression out of two expressions.
     */
    inline RegularExpr operator|(const RegularExpr& lhs, const RegularExpr& rhs) {
        return regex::altern(lhs, rhs);
    }


    /**
     * @related RegularExpr
     * @brief Create an alternation expression out of two expressions in place.
     */
    inline RegularExpr& operator|=(RegularExpr& lhs, const RegularExpr& rhs) {
        return lhs = lhs | rhs;
    }

}
