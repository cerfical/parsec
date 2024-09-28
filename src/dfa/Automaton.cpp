#include "dfa/Automaton.hpp"

#include "core/NameConflictError.hpp"

#include <boost/functional/hash.hpp>

#include <map>
#include <unordered_map>
#include <unordered_set>

namespace parsec::dfa {
    namespace {
        class StateItem {
        public:

            StateItem(const SymbolRule* rule, int pos) noexcept
                : rule_(rule), pos_(pos) {}


            const SymbolRule* rule() const noexcept {
                return rule_;
            }


            int pos() const noexcept {
                return pos_;
            }


            const Symbol& value() const {
                return rule_->body().posValue(pos_);
            }


            bool isAtEnd() const {
                return rule_->body().isEndPos(pos_);
            }


        private:
            const SymbolRule* rule_ = {};
            int pos_ = {};
        };


        using ItemSet = std::unordered_set<StateItem, boost::hash<StateItem>>;


        inline bool operator==(const StateItem& lhs, const StateItem& rhs) noexcept {
            return std::tuple(lhs.rule()->head(), lhs.pos()) == std::tuple(rhs.rule()->head(), rhs.pos());
        }


        const State emptyState;
    }
}


template <>
struct boost::hash<parsec::Symbol> {
    std::size_t operator()(const parsec::Symbol& symbol) const noexcept {
        return std::hash<parsec::Symbol>()(symbol);
    }
};


template <>
struct boost::hash<parsec::dfa::StateItem> {
    std::size_t operator()(const parsec::dfa::StateItem& item) const noexcept {
        return boost::hash_value(std::tuple(item.rule()->head(), item.pos()));
    }
};


namespace parsec::dfa {
    class Automaton::StateBuilder {
    public:

        StateBuilder(const SymbolGrammar& grammar, std::vector<State>& states)
            : grammar_(grammar), states_(states) {}


        void run() {
            if(auto startState = createStartState(); !startState.empty()) {
                const auto startStateId = buildState(std::move(startState));
                states_[startStateId].setStartState(true);
            }
        }


    private:
        ItemSet createStartState() const {
            ItemSet items;
            for(const auto& rule : grammar_.rules()) {
                for(const auto& pos : rule.body().firstPos()) {
                    items.emplace(&rule, pos);
                }
            }
            return items;
        }


        int buildState(ItemSet&& stateItems) {
            const auto [it, wasInserted] =
                itemSetsToIds_.emplace(std::move(stateItems), static_cast<int>(itemSetsToIds_.size()));
            const auto& [itemSet, stateId] = *it;

            if(wasInserted) {
                states_.emplace_back(stateId);
                buildTransitions(itemSet, stateId);
            }
            return stateId;
        }


        void buildTransitions(const ItemSet& stateItems, int stateId) {
            // use map to lexicographically sort transitions by their labels
            std::map<Symbol, ItemSet> transitions;

            for(const auto& item : stateItems) {
                if(item.isAtEnd()) {
                    if(!states_[stateId].isMatchState()) {
                        states_[stateId].setMatchedRule(item.rule()->head());
                        continue;
                    }
                    throw NameConflictError(states_[stateId].matchedRule(), item.rule()->head());
                }

                auto& itemTrans = transitions[item.value()];
                for(const auto& pos : item.rule()->body().followPos(item.pos())) {
                    itemTrans.emplace(item.rule(), pos);
                }
            }

            for(auto& [transLabel, transTarget] : transitions) {
                const auto transTargetId = buildState(std::move(transTarget));
                states_[stateId].addTransition(transTargetId, transLabel);
            }
        }


        std::unordered_map<ItemSet, int, boost::hash<ItemSet>> itemSetsToIds_;

        const SymbolGrammar& grammar_;
        std::vector<State>& states_;
    };


    Automaton::Automaton(const SymbolGrammar& grammar) {
        StateBuilder(grammar, states_).run();
    }


    Automaton::Automaton(const SymbolRule& pattern) {
        // create a singleton grammar containing the pattern as the only defined symbol
        SymbolGrammar grammar;
        grammar.define(pattern.head(), pattern.body());

        StateBuilder(grammar, states_).run();
    }


    const State& Automaton::stateById(int state) const noexcept {
        if(state >= 0 && state < states_.size()) {
            return states_[state];
        }
        return emptyState;
    }


    const State& Automaton::startState() const noexcept {
        if(!states_.empty()) {
            return states_.front();
        }
        return emptyState;
    }
}
