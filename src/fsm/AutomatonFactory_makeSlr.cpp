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

			Item(const fg::GrammarSymbol* symbol, std::size_t pos) noexcept
				: m_symbol(symbol), m_pos(pos) {
			}


			const fg::GrammarSymbol* headSymbol() const noexcept {
				return m_symbol;
			}

			const std::string& posSymbol() const noexcept {
				return rule().symbolAt(m_pos);
			}

			const fg::RuleExpr& rule() const noexcept {
				return m_symbol->rule();
			}


			std::size_t pos() const noexcept {
				return m_pos;
			}

			bool isAtEnd() const noexcept {
				return posSymbol().empty();
			}


		private:
			const fg::GrammarSymbol* m_symbol= {};
			std::size_t m_pos = {};
		};

		using ItemSet = std::unordered_set<Item, boost::hash<Item>>;



		inline bool operator==(const Item& lhs, const Item& rhs) noexcept {
			return lhs.headSymbol()->id() == rhs.headSymbol()->id() && lhs.pos() == rhs.pos();
		}

		inline std::size_t hash_value(const Item& item) noexcept {
			return boost::hash_value(std::tuple(item.headSymbol()->id(), item.pos()));
		}



		class ConstructSlr {
		public:
			explicit ConstructSlr(const fg::Grammar& grammar)
				: m_grammar(grammar) {
			}

			Automaton operator()() {
				if(m_grammar.startSymbol()) {
					ItemSet startItems;
					for(const auto& pos : m_grammar.startSymbol()->rule().firstPos()) {
						startItems.emplace(m_grammar.startSymbol(), pos);
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
					if(const auto symbol = m_grammar.lookupSymbol(item->posSymbol())) {
						for(const auto& pos : symbol->rule().firstPos()) {
							unexpanded.emplace(symbol, pos);
						}
					}
				}

				return expanded;
			}

			int createState(ItemSet&& stateItems) {
				const auto [it, wasInserted] = m_states.emplace(
					std::move(stateItems),
					static_cast<int>(m_states.size())
				);
				auto& [items, state] = *it;

				if(wasInserted) {
					buildStateTrans(items, state);
				}
				return state;
			}

			void buildStateTrans(const ItemSet& items, int state) {
				std::unordered_map<std::string, ItemSet> trans;
				for(const auto& item : computeClosure(items)) {
					if(item.isAtEnd()) {
						const bool wasInserted = m_acceptStates.insert(state).second;
						if(!wasInserted) {
							throw std::runtime_error("conflicting grammar rules");
						}
						m_slrBuilder.acceptState(state, item.headSymbol()->name());
						continue;
					}

					auto& itemTrans = trans[item.posSymbol()];
					for(const auto& pos : item.rule().followPos(item.pos())) {
						itemTrans.emplace(item.headSymbol(), pos);
					}
				}

				for(auto& [inputSymbol, stateItems] : trans) {
					const auto dest = createState(std::move(stateItems));
					m_slrBuilder.transition(inputSymbol, state, dest);
				}
			}

			std::unordered_map<ItemSet, int, boost::hash<ItemSet>> m_states;
			std::unordered_set<int> m_acceptStates;
			AutomatonBuilder m_slrBuilder;
			const fg::Grammar& m_grammar;
		};
	}


	Automaton AutomatonFactory::makeSlr(const fg::Grammar& lang) {
		return ConstructSlr(lang)();
	}
}