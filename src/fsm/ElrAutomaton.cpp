#include "fsm/ElrAutomaton.hpp"

#include "core/NameConflictError.hpp"
#include "fsm/DfaStateGen.hpp"

#include <boost/functional/hash.hpp>

#include <map>
#include <unordered_map>
#include <unordered_set>

namespace parsec::fsm {
    namespace {
        struct StateItem {

            friend bool operator==(const StateItem& lhs, const StateItem& rhs) noexcept = default;

            int dfaState = {};
            int backlink = -1;
        };

        using ItemSet = std::unordered_set<StateItem, boost::hash<StateItem>>;


        struct DfaStateTrans {
            int target = {};
            Symbol label;
        };

        struct DfaState {
            std::vector<DfaStateTrans> transitions;
            Symbol match;
            int id = {};
        };


        class GenDfaStates : DfaStateGen::StateSink {
        public:

            GenDfaStates(std::vector<DfaState>& states, int baseStateId)
                : states_(states), baseStateId_(baseStateId) {}

            void run(const SymbolGrammar& grammar) {
                DfaStateGen()
                    .setInputGrammar(&grammar)
                    .setStateSink(this)
                    .generate();
            }

        private:
            void addState(int id) override {
                auto& s = states_.emplace_back();
                s.id = id + baseStateId_;
            }

            void addStateTransition(int state, int target, const Symbol& label) override {
                states_[state + baseStateId_].transitions.emplace_back(target + baseStateId_, label);
            }

            void setStateMatch(int state, const Symbol& match) override {
                states_[state + baseStateId_].match = match;
            }

            std::vector<DfaState>& states_;
            int baseStateId_ = {};
        };


        class TransNetwork {
        public:

            TransNetwork(const SymbolGrammar& grammar) {
                for(const auto& symbol : grammar.symbols()) {
                    if(const auto* const rule = grammar.resolve(symbol)) {
                        const auto startStateId = static_cast<int>(states_.size());
                        symbolToStartState_[symbol] = startStateId;

                        GenDfaStates(states_, startStateId)
                            .run(SymbolGrammar().define(symbol, *rule));
                    }
                }
            }


            const DfaState* stateById(int state) const noexcept {
                return &states_[state];
            }

            const DfaState* startState(const Symbol& symbol) const noexcept {
                const auto symbolToIndexIt = symbolToStartState_.find(symbol);
                if(symbolToIndexIt != symbolToStartState_.end()) {
                    return &states_[symbolToIndexIt->second];
                }
                return nullptr;
            }


        private:
            std::vector<DfaState> states_;
            std::unordered_map<Symbol, int> symbolToStartState_;
        };
    }
}


template <>
struct boost::hash<parsec::fsm::StateItem> {
    std::size_t operator()(const parsec::fsm::StateItem& item) const noexcept {
        return boost::hash_value(std::tuple(item.dfaState, item.backlink));
    }
};


namespace parsec::fsm {
    class ElrAutomaton::StateBuilder {
    public:

        StateBuilder(const SymbolGrammar& grammar, std::vector<ElrState>& states)
            : transNet_(grammar), grammar_(grammar), states_(states) {}

        void run() {
            if(const auto startState = createStartState(); !startState.empty()) {
                const auto startStateId = buildState(startState);
            }
        }

    private:
        ItemSet createStartState() {
            ItemSet startState;
            if(const auto* const root = grammar_.root()) {
                startState.insert({ .dfaState = transNet_.startState(*root)->id });
            }
            return startState;
        }


        int buildState(const ItemSet& itemSet) {
            const auto [it, ok] = stateIds_.try_emplace(closure(itemSet), static_cast<int>(stateIds_.size()));
            const auto& [stateItems, stateId] = *it;

            if(ok) {
                auto& state = states_.emplace_back(stateId);
                for(const auto& item : stateItems) {
                    state.addBacklink(item.backlink);
                }
                buildTransitions(stateItems, stateId);
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

            for(const auto& trans : transNet_.stateById(item.dfaState)->transitions) {
                if(const auto* startState = transNet_.startState(trans.label)) {
                    closeItem({ .dfaState = startState->id }, closure);
                }
            }
        }


        void buildTransitions(const ItemSet& items, int stateId) {
            // use map to lexicographically sort transitions by their labels
            std::map<Symbol, ItemSet> transitions;

            for(int itemId = 0; const auto& item : items) {
                const auto& dfaState = transNet_.stateById(item.dfaState);
                if(dfaState->match) {
                    if(!states_[stateId].isMatchState()) {
                        states_[stateId].setMatch(dfaState->match);
                        states_[stateId].setBacklink(itemId);
                    } else {
                        throw NameConflictError(states_[stateId].match(), dfaState->match);
                    }
                }

                for(const auto& trans : dfaState->transitions) {
                    transitions[trans.label].insert(StateItem(trans.target, itemId));
                }

                itemId++;
            }

            for(const auto& [transLabel, transTarget] : transitions) {
                const auto transTargetId = buildState(transTarget);
                if(grammar_.contains(transLabel)) {
                    states_[stateId].addGoto(transTargetId, transLabel);
                } else {
                    states_[stateId].addShift(transTargetId, transLabel);
                }
            }
        }


        std::unordered_map<ItemSet, int, boost::hash<ItemSet>> stateIds_;
        TransNetwork transNet_;

        const SymbolGrammar& grammar_;
        std::vector<ElrState>& states_;
    };


    ElrAutomaton::ElrAutomaton(const SymbolGrammar& grammar) {
        StateBuilder(grammar, states_).run();
    }
}
