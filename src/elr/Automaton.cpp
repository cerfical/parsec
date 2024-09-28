#include "elr/Automaton.hpp"

#include "core/NameConflictError.hpp"

#include "dfa/Automaton.hpp"

#include <boost/functional/hash.hpp>

#include <map>
#include <set>
#include <unordered_map>

namespace parsec::elr {
    namespace {
        class StateItem {
        public:

            friend auto operator<=>(const StateItem&, const StateItem&) noexcept = default;


            StateItem(int dfaState, int backLink = -1) noexcept
                : dfaState_(dfaState), backLink_(backLink) {}


            int dfaState() const noexcept {
                return dfaState_;
            }


            int backLink() const noexcept {
                return backLink_;
            }


        private:
            int backLink_ = {};
            int dfaState_ = {};
        };


        using ItemSet = std::set<StateItem>;


        class TransNetwork {
        public:

            TransNetwork() = default;

            explicit TransNetwork(const SymbolGrammar& grammar) {
                for(const auto& rule : grammar.rules()) {
                    // skip empty rules
                    if(!rule) {
                        continue;
                    }

                    insertMachine(rule.head(), dfa::Automaton(rule));
                }
            }


            const dfa::State* stateById(int state) const noexcept {
                return &states_[state];
            }


            const dfa::State* startState(const Symbol& symbol) const noexcept {
                const auto symbolToIndexIt = symbolToStartState_.find(symbol);
                if(symbolToIndexIt != symbolToStartState_.end()) {
                    return &states_[symbolToIndexIt->second];
                }
                return nullptr;
            }


            const std::vector<dfa::State>& states() const noexcept {
                return states_;
            }


            explicit operator bool() const noexcept {
                return !isEmpty();
            }


            bool isEmpty() const noexcept {
                return states_.empty();
            }


        private:
            void insertMachine(const Symbol& name, const dfa::Automaton& dfa) {
                const auto baseStateId = static_cast<int>(states_.size());
                for(const auto& dfaState : dfa.states()) {
                    // create a new state with a unique id for each DFA state
                    auto& state = states_.emplace_back(baseStateId + dfaState.id());
                    for(const auto& dfaTrans : dfaState.transitions()) {
                        state.addTransition(baseStateId + dfaTrans.target, dfaTrans.label);
                    }
                    state.setMatchedRule(dfaState.matchedRule());
                }

                const auto startState = baseStateId + dfa.startState().id();
                symbolToStartState_[name] = startState;
                states_[startState].setStartState(true);
            }


            std::vector<dfa::State> states_;
            std::unordered_map<Symbol, int> symbolToStartState_;
        };


        const State emptyState;
    }
}


template <>
struct boost::hash<parsec::elr::StateItem> {
    std::size_t operator()(const parsec::elr::StateItem& item) const noexcept {
        return boost::hash_value(std::tuple(item.dfaState(), item.backLink()));
    }
};


namespace parsec::elr {
    class Automaton::StateBuilder {
    public:

        StateBuilder(const SymbolGrammar& grammar, std::vector<State>& states)
            : grammar_(grammar), states_(states) {}


        void run() {
            transNet_ = TransNetwork(grammar_);
            if(const auto startState = createStartState(); !startState.empty()) {
                const auto startStateId = buildState(startState);
                states_[startStateId].setStartState(true);
            }
        }


    private:
        ItemSet createStartState() {
            ItemSet startState;
            if(const auto& root = grammar_.root()) {
                startState.insert(transNet_.startState(root.head())->id());
            }
            return startState;
        }


        int buildState(const ItemSet& stateItems) {
            const auto [it, wasInserted] =
                itemSetsToIds_.try_emplace(closure(stateItems), static_cast<int>(itemSetsToIds_.size()));
            const auto& [items, stateId] = *it;

            if(wasInserted) {
                auto& state = states_.emplace_back(stateId);
                for(const auto& item : items) {
                    state.addBackLink(item.backLink());
                }

                buildTransitions(items, stateId);
            }
            return stateId;
        }


        ItemSet closure(const ItemSet& items) const {
            ItemSet closure;
            for(const auto& item : items) {
                closeItem(item, closure);
            }
            return closure;
        }


        void closeItem(const StateItem& item, ItemSet& closure) const {
            if(closure.contains(item)) {
                return;
            }
            closure.insert(item);

            for(const auto& trans : transNet_.stateById(item.dfaState())->transitions()) {
                if(const auto* startState = transNet_.startState(trans.label)) {
                    closeItem(startState->id(), closure);
                }
            }
        }


        void buildTransitions(const ItemSet& items, int stateId) {
            // use map to lexicographically sort transitions by their labels
            std::map<Symbol, ItemSet> transitions;

            for(int itemId = 0; const auto& item : items) {
                const auto& dfaState = transNet_.stateById(item.dfaState());
                if(dfaState->isMatchState()) {
                    if(!states_[stateId].isReduceState()) {
                        states_[stateId].setReduction(dfaState->matchedRule(), itemId);
                    } else {
                        throw NameConflictError(states_[stateId].reduction().reduceRule, dfaState->matchedRule());
                    }
                }

                for(const auto& trans : dfaState->transitions()) {
                    transitions[trans.label].insert(StateItem(trans.target, itemId));
                }

                itemId++;
            }

            for(const auto& [transLabel, transTarget] : transitions) {
                const auto transTargetId = buildState(transTarget);

                // check whether the symbol is a nonterminal symbol
                if(grammar_.contains(transLabel)) {
                    states_[stateId].addGoto(transTargetId, transLabel);
                } else {
                    states_[stateId].addShift(transTargetId, transLabel);
                }
            }
        }


        std::unordered_map<ItemSet, int, boost::hash<ItemSet>> itemSetsToIds_;
        TransNetwork transNet_;

        const SymbolGrammar& grammar_;
        std::vector<State>& states_;
    };


    Automaton::Automaton(const SymbolGrammar& grammar) {
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
