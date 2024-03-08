#include "fsm/AutomatonFactory.hpp"
#include "fsm/AutomatonBuilder.hpp"

#include <boost/functional/hash.hpp>
#include <unordered_set>
#include <stack>

namespace parsec::fsm {
	namespace {
		class Item {
		public:

			Item(std::string_view headSymbol, const fg::Rule* rule, Index pos) noexcept
				: m_headSymbol(headSymbol), m_rule(rule), m_pos(pos) {
			}


			std::string_view headSymbol() const noexcept {
				return m_headSymbol;
			}

			std::string_view posSymbol() const {
				return rule()->symbolAt(posIndex());
			}

			const fg::Rule* rule() const noexcept {
				return m_rule;
			}


			Index posIndex() const noexcept {
				return m_pos;
			}

			bool isAtEnd() const {
				return posSymbol().empty();
			}


		private:
			std::string_view m_headSymbol;
			const fg::Rule* m_rule = {};
			Index m_pos = {};
		};

		using ItemSet = std::unordered_set<Item, boost::hash<Item>>;



		inline bool operator==(const Item& lhs, const Item& rhs) noexcept {
			return std::tuple(lhs.headSymbol(), lhs.posIndex()) == std::tuple(rhs.headSymbol(), rhs.posIndex());
		}

		inline std::size_t hash_value(const Item& item) noexcept {
			return boost::hash_value(std::tuple(item.headSymbol(), item.posIndex()));
		}



		class MakeFsm {
		public:

			Automaton operator()(const fg::SymbolGrammar& grammar) {
				m_grammar = &grammar;

				if(auto startState = createStartState(); !startState.empty()) {
					createState(std::move(startState));
					m_fsmBuilder.startState(0);
				}

				return m_fsmBuilder.build();
			}

		protected:
			const fg::SymbolGrammar& inputGrammar() const noexcept {
				return *m_grammar;
			}

		private:
			virtual const ItemSet& computeClosure(const ItemSet& items) const = 0;

			virtual ItemSet createStartState() const = 0;


			Id createState(ItemSet&& stateItems) {
				const auto [it, wasInserted] = m_states.emplace(std::move(stateItems), m_states.size());
				const auto& [items, state] = *it;

				if(wasInserted) {
					buildStateTrans(items, state);
				}
				return state;
			}

			void buildStateTrans(const ItemSet& items, Id state) {
				std::unordered_map<std::string_view, ItemSet> trans;
				for(const auto& item : computeClosure(items)) {
					if(item.isAtEnd()) {
						if(const bool wasInserted = m_acceptStates.insert(state).second) {
							m_fsmBuilder.acceptState(state, std::string(item.headSymbol()));
							continue;
						}
						throw std::runtime_error("conflicting rules");
					}

					auto& itemTrans = trans[item.posSymbol()];
					for(const auto& pos : item.rule()->followPos(item.posIndex())) {
						itemTrans.emplace(item.headSymbol(), item.rule(), pos);
					}
				}

				for(auto& [inputSymbol, stateItems] : trans) {
					const auto dest = createState(std::move(stateItems));
					m_fsmBuilder.transition(
						std::string(inputSymbol),
						state,
						dest
					);
				}
			}


			std::unordered_map<ItemSet, Id, boost::hash<ItemSet>> m_states;
			std::unordered_set<Id> m_acceptStates;
			AutomatonBuilder m_fsmBuilder;

			const fg::SymbolGrammar* m_grammar = {};
		};



		class MakeDfa : public MakeFsm {
		private:
			const ItemSet& computeClosure(const ItemSet& items) const override {
				return items;
			}

			ItemSet createStartState() const override {
				ItemSet items;

				for(const auto& symbol : inputGrammar().symbols()) {
					const auto rule = inputGrammar().resolveSymbol(symbol);
					if(!rule) {
						continue;
					}

					for(const auto& pos : rule->firstPos()) {
						items.emplace(symbol, rule, pos);
					}
				}

				return items;
			}
		};



		class MakeSlr : public MakeFsm {
		private:
			const ItemSet& computeClosure(const ItemSet& items) const override {
				std::stack<Item> unexpanded;
				m_expandedItems.clear();

				// at start, every symbol is considered 'unexpanded', that is, each of them must be processed
				for(const auto& item : items) {
					unexpanded.push(item);
				}

				while(!unexpanded.empty()) {
					const auto [item, wasInserted] = m_expandedItems.emplace(unexpanded.top());
					unexpanded.pop();

					if(!wasInserted || item->isAtEnd()) {
						continue;
					}

					// expand only nonterminal symbols
					if(const auto rule = inputGrammar().resolveSymbol(item->posSymbol())) {
						for(const auto& pos : rule->firstPos()) {
							unexpanded.emplace(item->posSymbol(), rule, pos);
						}
					}
				}

				return m_expandedItems;
			}

			ItemSet createStartState() const override {
				ItemSet items;

				if(const auto rootRule = inputGrammar().resolveSymbol(inputGrammar().startSymbol())) {
					for(const auto& pos : rootRule->firstPos()) {
						items.emplace(inputGrammar().startSymbol(), rootRule, pos);
					}
				}

				return items;
			}


			mutable ItemSet m_expandedItems;
		};
	}


	Automaton AutomatonFactory::makeDfa(const fg::SymbolGrammar& regLang) {
		return MakeDfa()(regLang);
	}


	Automaton AutomatonFactory::makeSlr(const fg::SymbolGrammar& lang) {
		return MakeSlr()(lang);
	}


	AutomatonFactory& AutomatonFactory::get() {
		static AutomatonFactory factory;
		return factory;
	}
}