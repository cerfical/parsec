#include "core/SymbolGrammar.hpp"

#include "core/RegularExpr.hpp"
#include "core/Symbol.hpp"
#include "core/SymbolRule.hpp"

#include <algorithm>
#include <cstddef>
#include <ranges>
#include <utility>

namespace parsec {
    void SymbolGrammar::define(const Symbol& symbol, const RegularExpr& rule) {
        const auto [nameToRuleIdxIt, ok] = rulesCache_.try_emplace(symbol, rules_.size());
        if(!ok) {
            // combine all rules with the same head symbol into single rule
            auto& oldRule = rules_[nameToRuleIdxIt->second];
            oldRule = SymbolRule(oldRule.head(), oldRule.body() | rule);
            return;
        }

        try {
            symbols_.push_back(symbol);
        } catch(...) {
            rulesCache_.erase(nameToRuleIdxIt);
            throw;
        }

        try {
            rules_.emplace_back(symbol, rule);
        } catch(...) {
            symbols_.pop_back();
            rulesCache_.erase(nameToRuleIdxIt);
            throw;
        }

        symbolsSorted_ = false;
        rulesSorted_ = false;
    }


    const SymbolRule& SymbolGrammar::resolve(const Symbol& symbol) const noexcept {
        updateRulesCache();

        const auto nameToRuleIdxIt = rulesCache_.find(symbol);
        if(nameToRuleIdxIt == rulesCache_.end()) {
            static SymbolRule emptyRule;
            return emptyRule;
        }
        return rules_[nameToRuleIdxIt->second];
    }


    bool SymbolGrammar::contains(const Symbol& symbol) const noexcept {
        return rulesCache_.contains(symbol);
    }


    void SymbolGrammar::updateRulesCache() const noexcept {
        if(!cacheValid_) {
            for(size_t idx = 0; idx < rules_.size(); idx++) {
                rulesCache_[rules_[idx].head()] = idx;
            }
            cacheValid_ = true;
        }
    }


    const SymbolGrammar::SymbolList& SymbolGrammar::symbols() const noexcept {
        if(!symbolsSorted_) {
            std::ranges::sort(symbols_);
            symbolsSorted_ = true;
        }
        return symbols_;
    }


    const SymbolGrammar::RuleList& SymbolGrammar::rules() const noexcept {
        if(!rulesSorted_) {
            /*
             * Sorting will likely change the order of rules in the vector, and so the cached indices will refer to
             * rules with names other than the ones they originally referenced. Therefore, we invalidate the cache to
             * force a recalculation, if it is needed later.
             */
            cacheValid_ = false;

            std::ranges::sort(rules_, [](const auto& lhs, const auto& rhs) { return lhs.head() < rhs.head(); });
            rulesSorted_ = true;
        }
        return rules_;
    }
}
