#pragma once

#include "RegularExpr.hpp"
#include "Symbol.hpp"

#include <memory>
#include <optional>
#include <unordered_map>
#include <vector>

namespace parsec {

    /**
     * @brief Describes a formal language in terms of symbols and relationships between them.
     */
    class SymbolGrammar {
    public:

        /**
         * @brief Define a grammar symbol as a combination of other symbols.
         */
        SymbolGrammar& define(const Symbol& symbol, const RegularExpr& rule) {
            const auto [it, ok] = rules_.try_emplace(symbol, rule);
            if(ok) {
                try {
                    symbols_.push_back(symbol);
                } catch(...) {
                    rules_.erase(it);
                    throw;
                }
            } else {
                if(it->second) {
                    // if the symbol already has a rule, then merge it with the new one
                    *it->second |= rule;
                } else {
                    // otherwise, assign the rule to the symbol
                    it->second = rule;
                }
            }
            return *this;
        }


        /**
         * @brief Define a new grammar symbol, if it is not already defined.
         */
        SymbolGrammar& define(const Symbol& symbol) {
            const auto [it, ok] = rules_.try_emplace(symbol, std::nullopt);
            if(ok) {
                try {
                    symbols_.push_back(symbol);
                } catch(...) {
                    rules_.erase(it);
                    throw;
                }
            }
            return *this;
        }


        /**
         * @brief Set a new root for the grammar.
         */
        SymbolGrammar& setRoot(const Symbol& root) {
            root_ = root;
            return *this;
        }


        /**
         * @brief Start symbol for the grammar, if any.
         */
        const Symbol* root() const noexcept {
            return root_ ? std::to_address(root_) : nullptr;
        }


        /**
         * @brief Resolve a symbol to its defining rule, if any.
         */
        const RegularExpr* resolve(const Symbol& symbol) const noexcept {
            if(const auto it = rules_.find(symbol); it != rules_.end() && it->second) {
                return std::to_address(it->second);
            }
            return nullptr;
        }


        /**
         * @brief List of all defined symbols.
         */
        const std::vector<Symbol>& symbols() const noexcept {
            return symbols_;
        }


        /**
         * @brief Check if the symbol is defined in the grammar.
         */
        bool contains(const Symbol& symbol) const noexcept {
            return rules_.contains(symbol);
        }


    private:
        std::unordered_map<Symbol, std::optional<RegularExpr>> rules_;
        std::vector<Symbol> symbols_;
        std::optional<Symbol> root_;
    };

}
