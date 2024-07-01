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

        using SymbolList = std::vector<Symbol>;
        using RuleList = std::vector<SymbolRule>;

        SymbolGrammar(const SymbolGrammar&) = delete;
        SymbolGrammar& operator=(const SymbolGrammar&) = delete;

        SymbolGrammar(SymbolGrammar&&) noexcept = default;
        SymbolGrammar& operator=(SymbolGrammar&&) noexcept = default;

        ~SymbolGrammar() = default;


        /** @{ */
        /**
         * @brief Construct an empty grammar.
         */
        SymbolGrammar() = default;


        /**
         * @brief Define a new rule for a language symbol.
         *
         * @param symbol The symbol to attach the rule.
         * @param rule A new inference rule for the symbol.
         */
        void define(const Symbol& symbol, const RegularExpr& rule);


        /**
         * @brief Get the definition of a rule for a symbol.
         */
        const SymbolRule& resolve(const Symbol& symbol) const noexcept;


        /**
         * @brief Check if the grammar contains any definition for a symbol.
         */
        bool contains(const Symbol& symbol) const noexcept;


        /**
         * @brief Set a new root for the grammar.
         */
        void setRoot(const Symbol& root) noexcept {
            root_ = root;
        }


        /**
         * @brief The most derived, i.e., starting, rule of the grammar.
         */
        const SymbolRule& root() const noexcept {
            return resolve(root_);
        }


        /**
         * @brief The list of all defined symbols.
         */
        const SymbolList& symbols() const noexcept;


        /**
         * @brief The list of all defined symbol rules.
         */
        const RuleList& rules() const noexcept;
        /** @} */


    private:
        void updateRulesCache() const noexcept;

        mutable std::unordered_map<Symbol, std::size_t> rulesCache_;
        mutable SymbolList symbols_;
        mutable RuleList rules_;

        mutable bool cacheValid_ = true;
        mutable bool symbolsSorted_ = true;
        mutable bool rulesSorted_ = true;

        Symbol root_;
    };

}
