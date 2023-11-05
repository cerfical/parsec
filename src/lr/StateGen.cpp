#include "lr/StateGen.hpp"

#include <boost/functional/hash.hpp>
#include <gsl/narrow>

#include <unordered_set>
#include <unordered_map>
#include <stack>

namespace parsec::lr {
	namespace {
		struct Item {
			/** @{ */
			friend bool operator==(const Item& lhs, const Item& rhs) noexcept {
				return lhs.atom() == rhs.atom() && lhs.symbols() == rhs.symbols();
			}

			friend std::size_t hash_value(const Item& item) {
				std::size_t seed = 0;
				boost::hash_combine(seed, boost::hash_value(item.atom()));
				boost::hash_combine(seed, boost::hash_value(item.symbols()));
				return seed;
			}
			/** @} */


			/** @{ */
			Item(
				const fg::Atom* atom,
				const fg::Symbol* symbol,
				int symbols = 0,
				int terminals = 0
			) noexcept
				: m_atom(atom)
				, m_symbol(symbol)
				, m_terminals(terminals)
				, m_symbols(symbols)
			{ }
			/** @} */


			/** @{ */
			const fg::Atom* atom() const noexcept {
				return m_atom;
			}

			const fg::Symbol* symbol() const noexcept {
				return m_symbol;
			}
			
			int terminals() const noexcept {
				return m_terminals;
			}

			int symbols() const noexcept {
				return m_symbols;
			}

			bool atEnd() const noexcept {
				return m_atom == m_symbol->ruleEnd();
			}
			/** @} */


		private:
			const fg::Atom* m_atom;
			const fg::Symbol* m_symbol;
			int m_terminals;
			int m_symbols;
		};

		using ItemSet = std::unordered_set<Item, boost::hash<Item>>;


		class RunImpl {
		public:
			explicit RunImpl(const fg::Grammar& grammar)
				: m_grammar(grammar)
			{ }

			StateList operator()() {
				const auto start = m_grammar.startSymbol();
				ItemSet startItems;

				// construct the initial state from the first symbols of each rule
				for(const auto atom : start->rule()->leadingAtoms()) {
					startItems.emplace(atom, start);
				}

				createState(std::move(startItems), nullptr);
				while(!m_unprocessed.empty()) {
					const auto& [items, id] = *m_unprocessed.top();
					m_unprocessed.pop();
					
					processState(
						items,
						id
					);
				}

				return m_states;
			}

		private:
			ItemSet computeClosure(const ItemSet& items) const {
				std::stack<Item> unexpanded;
				ItemSet closure;

				// fill the stack with items to be expanded
				for(const auto& item : items) {
					unexpanded.push(item);
				}

				// expand all nonterminal symbols
				while(!unexpanded.empty()) {
					// if the item was already processed, skip it
					const auto [item, wasInserted] = closure.emplace(unexpanded.top());
					if(unexpanded.pop(); !wasInserted) {
						continue;
					}

					// skip end markers
					if(item->atEnd()) {
						continue;
					}

					// skip any undefined, terminal and end symbols
					const auto sym = m_grammar.symbolByName(item->atom()->value());
					if(!sym || sym->isTerminal() || sym->isEnd()) {
						continue;
					}

					// all atoms forming the beginning of some rule must also be processed
					for(const auto atom : sym->rule()->leadingAtoms()) {
						unexpanded.emplace(atom, sym);
					}
				}

				return closure;
			}

			int createState(ItemSet&& stateItems, const fg::Symbol* symbol) {
				const auto newId = gsl::narrow_cast<int>(m_states.size()); // unique identifier for a new state
				const auto [it, wasInserted] = m_stateIds.emplace(std::move(stateItems), newId);
				const auto& [items, id] = *it;

				// construct a new state only if it differs from the previously created ones
				if(wasInserted) {
					m_states.emplace_back(newId, symbol);
					m_unprocessed.push(std::to_address(it));
				}

				return id;
			}

			void processState(const ItemSet& items, int id) {
				std::unordered_map<const fg::Symbol*, ItemSet> stateGoto;
				for(const auto& item : computeClosure(items)) {
					// if we have reached the end of the rule, add a reduce action
					if(item.atEnd()) {
						m_states[id].addReduction(
							item.symbol(),
							item.symbols(),
							item.terminals()
						);
						continue;
					}

					// otherwise, construct the next state to transition to on the input symbol
					for(const auto atom : item.atom()->nextAtoms()) {
						if(const auto sym = m_grammar.symbolByName(item.atom()->value())) {
							stateGoto[sym].emplace(
								atom,
								item.symbol(),
								item.symbols() + 1,
								item.terminals() + (sym->isTerminal() ? 1 : 0)
							);
						}
					}
				}

				// add shift actions for all generated transitions
				for(auto& [symbol, items] : stateGoto) {
					const auto newState = createState(std::move(items), symbol);
					m_states[id].addShift(
						symbol,
						newState
					);
				}
			}


			using StateIdTable = std::unordered_map<ItemSet, int, boost::hash<ItemSet>>;

			std::stack<const StateIdTable::value_type*> m_unprocessed;
			StateIdTable m_stateIds;
			StateList m_states;

			const fg::Grammar& m_grammar;
		};
	}


	StateList StateGen::run(const fg::Grammar& grammar) {
		return RunImpl(grammar)();
	}
}