module;

#include <boost/functional/hash.hpp>

#include <unordered_map>
#include <unordered_set>

module parsec.fsm;

import parsec.bnf;

namespace parsec::fsm {
    namespace {
        struct Item;
    }
}

template <>
struct boost::hash<parsec::fsm::Item> {
    std::size_t operator()(const auto& item) const noexcept {
        return boost::hash_value(std::tuple(item.dfaState, item.backlink));
    }
};

namespace parsec::fsm {
    namespace {
        struct Item {

            friend bool operator==(const Item& lhs, const Item& rhs) = default;

            int dfaState = {};
            int backlink = -1;
        };

        using ItemSet = std::unordered_set<Item, boost::hash<Item>>;


        struct DfaStateTrans {
            int target = {};
            bnf::Symbol label;
        };

        struct DfaState {
            std::vector<DfaStateTrans> transitions;
            bnf::Symbol match;
            int id = {};
        };


        class GenDfaStates : DfaStateGen::StateSink {
        public:

            GenDfaStates(std::vector<DfaState>* states, int baseStateId)
                : states_(states), baseStateId_(baseStateId) {}

            void run(const bnf::SymbolGrammar& grammar) {
                DfaStateGen()
                    .setInputGrammar(&grammar)
                    .setStateSink(this)
                    .generate();
            }

        private:
            void addState(int id) override {
                auto& s = states_->emplace_back();
                s.id = id + baseStateId_;
            }

            void addStateTransition(int state, int target, const bnf::Symbol& label) override {
                (*states_)[state + baseStateId_].transitions.emplace_back(target + baseStateId_, label);
            }

            void setStateMatch(int state, const bnf::Symbol& match) override {
                (*states_)[state + baseStateId_].match = match;
            }

            std::vector<DfaState>* states_;
            int baseStateId_ = {};
        };


        class TransNetwork {
        public:

            TransNetwork(const bnf::SymbolGrammar& grammar) {
                for(const auto& symbol : grammar.symbols()) {
                    if(const auto* const rule = grammar.resolve(symbol)) {
                        const auto startStateId = static_cast<int>(states_.size());
                        startStates_[symbol] = startStateId;

                        GenDfaStates(&states_, startStateId)
                            .run(bnf::SymbolGrammar().define(symbol, *rule));
                    }
                }
            }


            const DfaState* stateById(int state) const noexcept {
                return &states_[state];
            }

            const DfaState* startState(const bnf::Symbol& symbol) const noexcept {
                const auto symbolToIndexIt = startStates_.find(symbol);
                if(symbolToIndexIt != startStates_.end()) {
                    return &states_[symbolToIndexIt->second];
                }
                return nullptr;
            }


        private:
            std::vector<DfaState> states_;
            std::unordered_map<bnf::Symbol, int> startStates_;
        };


        class GenerateStates {
        public:

            GenerateStates(const bnf::SymbolGrammar& grammar, ElrStateGen::StateSink* sink)
                : transNet_(grammar), grammar_(grammar), sink_(sink) {}

            void run() {
                if(const auto startState = createStartState(); !startState.empty()) {
                    addState(startState);
                }
            }

        private:
            template <typename... Params, typename... Args>
            void sink(void (ElrStateGen::StateSink::*func)(Params...), Args&&... args) {
                if(sink_) {
                    (sink_->*func)(std::forward<Args>(args)...);
                }
            }


            ItemSet createStartState() {
                ItemSet startState;
                if(const auto* const root = grammar_.root()) {
                    startState.insert({ .dfaState = transNet_.startState(*root)->id });
                }
                return startState;
            }


            int addState(const ItemSet& state) {
                const auto [it, ok] = states_.try_emplace(closure(state), static_cast<int>(states_.size()));
                const auto& [items, id] = *it;
                if(ok) {
                    sink(&ElrStateGen::StateSink::addState, id);
                    for(const auto& item : items) {
                        sink(&ElrStateGen::StateSink::addStateBacklink, id, item.backlink);
                    }
                    addStateTransitions(items, id);
                }
                return id;
            }


            ItemSet closure(const ItemSet& items) const {
                ItemSet closure;
                for(const auto& item : items) {
                    closeItem(item, closure);
                }
                return closure;
            }

            void closeItem(const Item& item, ItemSet& closure) const {
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


            void addStateTransitions(const ItemSet& items, int id) {
                std::unordered_map<bnf::Symbol, ItemSet> transitions;
                bnf::Symbol match;

                for(int itemId = 0; const auto& item : items) {
                    const auto& dfaState = transNet_.stateById(item.dfaState);
                    if(dfaState->match) {
                        if(!match) {
                            sink(&ElrStateGen::StateSink::setStateMatch, id, dfaState->match);
                            sink(&ElrStateGen::StateSink::setActiveBacklink, id, itemId);
                            match = dfaState->match;
                        } else {
                            throw NameConflictError(match, dfaState->match);
                        }
                    }

                    for(const auto& trans : dfaState->transitions) {
                        transitions[trans.label].emplace(trans.target, itemId);
                    }

                    itemId++;
                }

                for(const auto& [label, transTarget] : transitions) {
                    const auto targetId = addState(transTarget);
                    const auto addTrans = grammar_.contains(label)
                                            ? &ElrStateGen::StateSink::addStateRuleTransition
                                            : &ElrStateGen::StateSink::addStateTokenTransition;
                    sink(addTrans, id, targetId, label);
                }
            }


            std::unordered_map<ItemSet, int, boost::hash<ItemSet>> states_;
            TransNetwork transNet_;

            const bnf::SymbolGrammar& grammar_;
            ElrStateGen::StateSink* sink_ = {};
        };
    }


    void ElrStateGen::generate() {
        if(grammar_) {
            GenerateStates(*grammar_, sink_)
                .run();
        }
    }
}
