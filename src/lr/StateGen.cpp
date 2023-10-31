#include "lr/StateGen.hpp"
#include "fg/rules.hpp"

#include <unordered_set>
#include <unordered_map>
#include <stack>

#include <boost/functional/hash.hpp>
#include <gsl/narrow>

namespace parsec::lr {
	namespace {
		/** @{ */
		class FindStartSymbols : fg::AtomVisitor {
		public:
			/** @{ */
			explicit FindStartSymbols(const fg::Grammar& grammar) noexcept
				: m_grammar(grammar)
			{ }
			/** @} */

			/** @{ */
			std::vector<const fg::Symbol*> operator()() {
				// find all symbols that have no references
				m_startSymbols.resize(m_grammar.symbols().size(), true);
				for(const auto& sym : m_grammar.symbols()) {
					if(!sym.terminal()) {
						m_currentSymbol = &sym;
						sym.ruleBody()->traverse(*this);
					} else {
						// terminal symbols cannot be start symbols
						m_startSymbols[sym.id()] = false;
					}
				}

				// collect all found starting symbols into one list
				std::vector<const fg::Symbol*> syms;
				for(std::size_t i = 0; i < m_startSymbols.size(); i++) {
					if(m_startSymbols[i]) {
						syms.push_back(&m_grammar.symbols()[i]);
					}
				}
				return syms;
			}
			/** @} */

		private:
			/** @{ */
			void visit(const fg::Atom& n) override {
				const auto sym = m_grammar.lookupSymbol(n.value());
				if(sym != m_currentSymbol) {
					m_startSymbols[sym->id()] = false;
				}
			}
			/** @} */

			/** @{ */
			const fg::Symbol* m_currentSymbol = nullptr;
			std::vector<bool> m_startSymbols;
			const fg::Grammar& m_grammar;
			/** @} */
		};
		/** @} */


		/** @{ */
		struct Item {
			/** @{ */
			friend bool operator==(const Item& lhs, const Item& rhs) noexcept {
				return lhs.atom == rhs.atom;
			}

			friend std::size_t hash_value(const Item& item) {
				return boost::hash_value(item.atom);
			}
			/** @} */


			/** @{ */
			const fg::Atom* atom = nullptr;
			const fg::Symbol* symbol = nullptr;
			int pos = 0;
			/** @} */
		};

		using ItemSet = std::unordered_set<Item, boost::hash<Item>>;
		/** @} */


		/** @{ */
		class RunImpl {
		public:
			/** @{ */
			explicit RunImpl(const fg::Grammar& grammar) noexcept
				: m_grammar(grammar)
			{ }
			/** @} */


			/** @{ */
			StateList operator()() {
				putState(createStartState());

				while(!m_unprocessed.empty()) {
					const auto state = m_unprocessed.top();
					m_unprocessed.pop();
					processState(state->first, state->second);
				}

				return std::move(m_states);
			}
			/** @} */


		private:
			/** @{ */
			using StateIdTable = std::unordered_map<ItemSet, int, boost::hash<ItemSet>>;
			using StateHandle = const StateIdTable::value_type*;
			/** @} */


			/** @{ */
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

					// skip terminal and end marker symbols
					const auto sym = m_grammar.lookupSymbol(item->atom->value());
					if(item->atom == item->symbol->ruleEnd() || sym->terminal()) {
						continue;
					}

					// all atoms forming the beginning of some rule must also be processed
					for(const auto atom : sym->ruleBody()->leadingAtoms()) {
						unexpanded.emplace(atom, sym, 0);
					}
				}

				return closure;
			}

			ItemSet createStartState() {
				ItemSet items;
				for(const auto sym : FindStartSymbols(m_grammar)()) {
					for(const auto atom : sym->ruleBody()->leadingAtoms()) {
						items.emplace(atom, sym, 0);
					}
				}
				return items;
			}
			/** @} */


			/** @{ */
			State& putState(ItemSet&& stateItems) {
				const auto newId = gsl::narrow_cast<int>(m_states.size()); // unique identifier for a new state
				const auto [it, wasInserted] = m_stateIds.emplace(std::move(stateItems), newId);
				const auto state = &(*it);

				// construct a new state only if it differs from the previously created ones
				if(wasInserted) {
					m_states.emplace_back(newId);
					m_unprocessed.push(state);
				}

				return m_states[state->second];
			}

			void processState(const ItemSet& items, int id) {
				std::unordered_map<const fg::Symbol*, ItemSet> stateGoto;

				for(const auto& item : computeClosure(items)) {
					// if we have reached the end of the rule, add a reduce action
					if(item.atom == item.symbol->ruleEnd()) {
						m_states[id].addReduction(item.symbol, item.pos);
						continue;
					}

					// otherwise, construct the next state to transition to on the input symbol
					for(const auto atom : item.atom->nextAtoms()) {
						const auto sym = m_grammar.lookupSymbol(item.atom->value());
						stateGoto[sym].emplace(
							atom, item.symbol, item.pos + 1
						);
					}
				}

				// add shift actions for all generated transitions
				for(auto& [symbol, items] : stateGoto) {
					const auto& newState = putState(std::move(items));
					m_states[id].addShift(symbol, newState.id());
				}
			}
			/** @} */


			/** @{ */
			std::stack<StateHandle> m_unprocessed;
			StateIdTable m_stateIds;
			StateList m_states;
			/** @} */

			/** @{ */
			const fg::Grammar& m_grammar;
			/** @} */
		};
		/** @} */
	}


	StateList StateGen::run(const fg::Grammar& grammar) {
		return RunImpl(grammar)();
	}
}