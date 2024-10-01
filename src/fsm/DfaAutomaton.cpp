#include "fsm/DfaAutomaton.hpp"

#include "core/NameConflictError.hpp"

#include <boost/functional/hash.hpp>

#include <map>
#include <unordered_map>
#include <unordered_set>

namespace parsec::fsm {
    namespace {
        struct StateItem {

            friend bool operator==(const StateItem& lhs, const StateItem& rhs) noexcept {
                return std::tuple(lhs.rule->head(), lhs.pos) == std::tuple(rhs.rule->head(), rhs.pos);
            }


            const Symbol& value() const {
                return rule->body().posValue(pos);
            }

            bool isAtEnd() const {
                return rule->body().isEndPos(pos);
            }


            const SymbolRule* rule = {};
            int pos = {};
        };

        using ItemSet = std::unordered_set<StateItem, boost::hash<StateItem>>;
    }
}


template <>
struct boost::hash<parsec::Symbol> {
    std::size_t operator()(const parsec::Symbol& symbol) const noexcept {
        return std::hash<parsec::Symbol>()(symbol);
    }
};

template <>
struct boost::hash<parsec::fsm::StateItem> {
    std::size_t operator()(const parsec::fsm::StateItem& item) const noexcept {
        return boost::hash_value(std::tuple(item.rule->head(), item.pos));
    }
};


namespace parsec::fsm {
    class DfaAutomaton::StateBuilder {
    public:

        StateBuilder(const SymbolGrammar& grammar, std::vector<DfaState>& states)
            : grammar_(grammar), states_(states) {}

        void run() {
            if(auto startState = createStartState(); !startState.empty()) {
                buildState(std::move(startState));
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


        int buildState(ItemSet&& itemSet) {
            const auto [it, ok] = stateIds_.emplace(std::move(itemSet), static_cast<int>(stateIds_.size()));
            const auto& [stateItems, stateId] = *it;
            if(ok) {
                states_.emplace_back(stateId);
                buildTransitions(stateItems, stateId);
            }
            return stateId;
        }


        void buildTransitions(const ItemSet& stateItems, int stateId) {
            // use map to lexicographically sort transitions by their labels
            std::map<Symbol, ItemSet> transitions;

            for(const auto& item : stateItems) {
                if(item.isAtEnd()) {
                    if(!states_[stateId].isMatchState()) {
                        states_[stateId].setMatch(item.rule->head());
                        continue;
                    }
                    throw NameConflictError(states_[stateId].match(), item.rule->head());
                }

                auto& itemTrans = transitions[item.value()];
                for(const auto& pos : item.rule->body().followPos(item.pos)) {
                    itemTrans.emplace(item.rule, pos);
                }
            }

            for(auto& [transLabel, transTarget] : transitions) {
                const auto transTargetId = buildState(std::move(transTarget));
                states_[stateId].addTransition(transTargetId, transLabel);
            }
        }


        std::unordered_map<ItemSet, int, boost::hash<ItemSet>> stateIds_;

        const SymbolGrammar& grammar_;
        std::vector<DfaState>& states_;
    };


    DfaAutomaton::DfaAutomaton(const SymbolGrammar& grammar) {
        StateBuilder(grammar, states_).run();
    }
}
