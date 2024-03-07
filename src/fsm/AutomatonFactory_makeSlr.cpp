#include "fsm/AutomatonFactory.hpp"
#include "fsm/AutomatonBuilder.hpp"

#include <boost/functional/hash.hpp>

#include <unordered_set>
#include <unordered_map>
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

			std::string_view bodySymbol() const {
				return rule()->symbolAt(pos());
			}

			const fg::Rule* rule() const noexcept {
				return m_rule;
			}


			Index pos() const noexcept {
				return m_pos;
			}

			bool isAtEnd() const {
				return bodySymbol().empty();
			}


		private:
			std::string_view m_headSymbol;
			const fg::Rule* m_rule = {};
			Index m_pos = {};
		};

		using ItemSet = std::unordered_set<Item, boost::hash<Item>>;



		inline bool operator==(const Item& lhs, const Item& rhs) noexcept {
			return std::tuple(lhs.headSymbol(), lhs.pos()) == std::tuple(rhs.headSymbol(), rhs.pos());
		}

		inline std::size_t hash_value(const Item& item) noexcept {
			return boost::hash_value(std::tuple(item.headSymbol(), item.pos()));
		}



		class ConstructSlr {
		public:
			explicit ConstructSlr(const fg::SymbolGrammar& grammar)
				: m_grammar(grammar) {
			}

			Automaton operator()() {
				if(const auto rootRule = m_grammar.resolveSymbol(m_grammar.startSymbol())) {
					ItemSet startItems;
					for(const auto& pos : rootRule->firstPos()) {
						startItems.emplace(m_grammar.startSymbol(), rootRule, pos);
					}

					createState(std::move(startItems));
					m_slrBuilder.startState(0);
				}
				return m_slrBuilder.build();
			}

		private:
			ItemSet computeClosure(const ItemSet& items) const {
				std::stack<Item> unexpanded;
				ItemSet expanded;

				// at start, every symbol is considered 'unexpanded', that is, each of them must be processed
				for(const auto& item : items) {
					unexpanded.push(item);
				}

				while(!unexpanded.empty()) {
					const auto [item, wasInserted] = expanded.emplace(unexpanded.top());
					unexpanded.pop();

					if(!wasInserted || item->isAtEnd()) {
						continue;
					}

					// expand only nonterminal symbols
					if(const auto rule = m_grammar.resolveSymbol(item->bodySymbol())) {
						for(const auto& pos : rule->firstPos()) {
							unexpanded.emplace(item->bodySymbol(), rule, pos);
						}
					}
				}

				return expanded;
			}

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
						if(const bool wasInserted = m_acceptStates.insert(state).second; !wasInserted) {
							throw std::runtime_error("conflicting grammar rules");
						}
						m_slrBuilder.acceptState(state, std::string(item.headSymbol()));
						continue;
					}

					auto& itemTrans = trans[item.bodySymbol()];
					for(const auto& pos : item.rule()->followPos(item.pos())) {
						itemTrans.emplace(item.headSymbol(), item.rule(), pos);
					}
				}

				for(auto& [inputSymbol, stateItems] : trans) {
					const auto dest = createState(std::move(stateItems));
					m_slrBuilder.transition(std::string(inputSymbol), state, dest);
				}
			}

			std::unordered_map<ItemSet, Id, boost::hash<ItemSet>> m_states;
			std::unordered_set<Id> m_acceptStates;
			AutomatonBuilder m_slrBuilder;
			const fg::SymbolGrammar& m_grammar;
		};
	}


	Automaton AutomatonFactory::makeSlr(const fg::SymbolGrammar& lang) {
		return ConstructSlr(lang)();
	}
}