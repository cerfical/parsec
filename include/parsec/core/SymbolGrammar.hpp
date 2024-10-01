#pragma once

#include "RegularExpr.hpp"
#include "Symbol.hpp"
#include "SymbolRule.hpp"

#include <cstddef>
#include <unordered_map>
#include <vector>

namespace parsec {

    /**
     * @brief Defines a formal language out of symbols.
     */
    class SymbolGrammar {
    public:

        SymbolGrammar(const SymbolGrammar&) = delete;
        SymbolGrammar& operator=(const SymbolGrammar&) = delete;

        SymbolGrammar(SymbolGrammar&&) noexcept = default;
        SymbolGrammar& operator=(SymbolGrammar&&) noexcept = default;

        ~SymbolGrammar() noexcept = default;


        /** @{ */
        /**
         * @brief Construct an empty grammar.
         */
        SymbolGrammar() = default;


        /**
         * @brief Define a new rule for a language symbol.
         */
        void define(const Symbol& symbol, const RegularExpr& rule);


        /**
         * @brief Get the definition of a rule for a symbol.
         */
        const SymbolRule& resolve(const Symbol& symbol) const noexcept;


        /**
         * @brief Check if the grammar contains any definition for a symbol.
         */
        bool contains(const Symbol& symbol) const noexcept {
            return rulesIndex_.contains(symbol);
        }


        /**
         * @brief The most derived, i.e., starting, rule of the grammar.
         */
        const SymbolRule& root() const noexcept {
            return resolve(root_);
        }


        /**
         * @brief Set a new root for the grammar.
         */
        void setRoot(const Symbol& root) {
            root_ = root;
        }


        /**
         * @brief The list of all defined symbols.
         */
        const std::vector<Symbol>& symbols() const noexcept {
            return symbols_;
        }


        /**
         * @brief The list of all defined symbol rules.
         */
        const std::vector<SymbolRule>& rules() const noexcept {
            return rules_;
        }
        /** @} */


    private:
        std::unordered_map<Symbol, std::size_t> rulesIndex_;
        std::vector<Symbol> symbols_;
        std::vector<SymbolRule> rules_;

        Symbol root_;
    };

}
