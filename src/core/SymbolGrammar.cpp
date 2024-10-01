#include "core/SymbolGrammar.hpp"

#include "core/RegularExpr.hpp"
#include "core/Symbol.hpp"
#include "core/SymbolRule.hpp"

#include <ranges>
#include <utility>

namespace parsec {
    void SymbolGrammar::define(const Symbol& symbol, const RegularExpr& rule) {
        const auto [ruleIdxIt, ok] = rulesIndex_.try_emplace(symbol, rules_.size());
        if(!ok) {
            // combine all rules with the same head symbol
            auto& oldRule = rules_[ruleIdxIt->second];
            oldRule = SymbolRule(oldRule.head(), oldRule.body() | rule);
            return;
        }

        try {
            symbols_.push_back(symbol);
            try {
                rules_.emplace_back(symbol, rule);
            } catch(...) {
                symbols_.pop_back();
                throw;
            }
        } catch(...) {
            rulesIndex_.erase(ruleIdxIt);
            throw;
        }
    }

    const SymbolRule& SymbolGrammar::resolve(const Symbol& symbol) const noexcept {
        const auto it = rulesIndex_.find(symbol);
        if(it == rulesIndex_.end()) {
            static SymbolRule emptyRule;
            return emptyRule;
        }
        return rules_[it->second];
    }
}
