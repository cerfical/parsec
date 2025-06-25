module;

#include "bnf/SymbolGrammar.hpp"

#include <boost/functional/hash.hpp>

#include <optional>
#include <unordered_map>
#include <unordered_set>

module parsec.fsm;

namespace parsec::fsm {
    namespace {
        struct Item;
    }
}

template <>
struct boost::hash<parsec::fsm::Item> {
    std::size_t operator()(const auto& item) const noexcept {
        return boost::hash_value(std::tuple(item.rule, item.pos));
    }
};

namespace parsec::fsm {
    namespace {
        struct Item {

            friend bool operator==(const Item& lhs, const Item& rhs) noexcept {
                return std::tuple(lhs.rule, lhs.pos) == std::tuple(rhs.rule, rhs.pos);
            }

            const bnf::Symbol* value() const {
                return rule->valueAt(pos);
            }

            bool isAtEnd() const {
                return rule->isEndPos(pos);
            }

            bnf::Symbol symbol;
            const bnf::RegularExpr* rule = {};
            int pos = {};
        };

        using ItemSet = std::unordered_set<Item, boost::hash<Item>>;


        class GenerateStates {
        public:

            GenerateStates(DfaStateGen::StateSink* sink)
                : sink_(sink) {}

            void run(const bnf::SymbolGrammar& grammar) {
                if(auto startState = createStartState(grammar); !startState.empty()) {
                    addState(std::move(startState));
                }
            }

        private:
            static ItemSet createStartState(const bnf::SymbolGrammar& grammar) {
                ItemSet startState;
                for(const auto& symbol : grammar.symbols()) {
                    if(const auto* const rule = grammar.resolve(symbol)) {
                        for(const auto& pos : rule->firstPos()) {
                            startState.emplace(symbol, rule, pos);
                        }
                    }
                }
                return startState;
            }


            template <typename... Params, typename... Args>
            void sink(void (DfaStateGen::StateSink::*func)(Params...), Args&&... args) {
                if(sink_) {
                    (sink_->*func)(std::forward<Args>(args)...);
                }
            }


            int addState(ItemSet&& state) {
                const auto [it, ok] = states_.emplace(std::move(state), static_cast<int>(states_.size()));
                const auto& [items, id] = *it;
                if(ok) {
                    sink(&DfaStateGen::StateSink::addState, id);
                    addStateTransitions(items, id);
                }
                return id;
            }

            void addStateTransitions(const ItemSet& items, int id) {
                std::unordered_map<bnf::Symbol, ItemSet> transitions;
                bnf::Symbol match;

                for(const auto& item : items) {
                    if(item.isAtEnd()) {
                        if(!match) {
                            sink(&DfaStateGen::StateSink::setStateMatch, id, item.symbol);
                            match = item.symbol;
                            continue;
                        }
                        throw NameConflictError(match, item.symbol);
                    }

                    for(const auto& pos : item.rule->followPos(item.pos)) {
                        transitions[*item.value()].emplace(item.symbol, item.rule, pos);
                    }
                }

                for(auto& [label, target] : transitions) {
                    const auto targetId = addState(std::move(target));
                    sink(&DfaStateGen::StateSink::addStateTransition, id, targetId, label);
                }
            }


            std::unordered_map<ItemSet, int, boost::hash<ItemSet>> states_;
            DfaStateGen::StateSink* sink_ = {};
        };
    }


    void DfaStateGen::generate() {
        if(grammar_) {
            GenerateStates(sink_)
                .run(*grammar_);
        }
    }
}
