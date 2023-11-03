#include "dfa/StateGen.hpp"

#include <boost/functional/hash.hpp>
#include <gsl/narrow>

#include <unordered_set>
#include <unordered_map>
#include <stack>
#include <map>

namespace parsec::dfa {
	namespace {
		/** @{ */
		struct Item {
			/** @{ */
			friend bool operator==(const Item& lhs, const Item& rhs) noexcept {
				return lhs.currentAtom() == rhs.currentAtom();
			}

			friend std::size_t hash_value(const Item& item) {
				return boost::hash_value(item.currentAtom());
			}
			/** @} */


			/** @{ */
			Item(const fg::Atom* atom, const fg::Symbol* symbol) noexcept
				: m_atom(atom), m_symbol(symbol)
			{ }
			/** @} */


			/** @{ */
			const fg::Atom* currentAtom() const noexcept {
				return m_atom;
			}

			const fg::Symbol* originSymbol() const noexcept {
				return m_symbol;
			}

			bool atEnd() const noexcept {
				return m_atom == m_symbol->ruleEnd();
			}
			/** @} */


		private:
			const fg::Atom* m_atom;
			const fg::Symbol* m_symbol;
		};

		using ItemSet = std::unordered_set<Item, boost::hash<Item>>;
		/** @} */


		/** @{ */
		class RunImpl {
		public:
			/** @{ */
			explicit RunImpl(const fg::Grammar& grammar)
				: m_grammar(grammar)
			{ }
			/** @} */
			
			
			/** @{ */
			StateList operator()() {
				if(auto startState = createStartState(); !startState.empty()) {
					putState(std::move(startState));
					while(!m_unprocessed.empty()) {
						const auto state = m_unprocessed.top();
						m_unprocessed.pop();
						processState(state->first, state->second);
					}
				}
				return m_states;
			}
			/** @} */


		private:
			/** @{ */
			using StateIdTable = std::unordered_map<ItemSet, int, boost::hash<ItemSet>>;
			using StateHandle = const StateIdTable::value_type*;
			/** @} */


			/** @{ */
			ItemSet createStartState() const {
				ItemSet items;
				
				// construct an initial state for DFA from the leading characters of all token rules
				for(const auto& sym : m_grammar.symbols()) {
					// skip nonterminals
					if(sym.isNonterminal()) {
						continue;
					}

					// the initial state will contain all the atoms that can form the beginning of some rule
					for(const auto atom : sym.rule()->leadingAtoms()) {
						items.emplace(atom, &sym);
					}
				}

				return items;
			}

			int putState(ItemSet&& stateItems) {

				const auto newId = gsl::narrow_cast<int>(m_states.size()); // unique identifier for a new state
				const auto [it, wasInserted] = m_stateIds.emplace(std::move(stateItems), newId);
				const auto& [items, id] = *it;

				// construct a new state only if it differs from the previously created ones
				if(wasInserted) {
					m_states.emplace_back(newId);
					m_unprocessed.push(std::to_address(it));
				}

				return id;
			}

			void processState(const ItemSet& items, int id) {
				// all items, that are located at the rule end, represent possible matches
				for(const auto& item : items) {
					if(item.atEnd()) {
						m_states[id].addMatch(item.originSymbol());
					}
				}

				// incrementally build up all states to which there is a transition
				std::map<char, ItemSet> nextStates;
				for(const auto& item : items) {
					for(const auto nextAtom : item.currentAtom()->nextAtoms()) {
						// just skip ill-defined atoms
						if(item.currentAtom()->value().size() != 1) {
							continue;
						}

						nextStates[item.currentAtom()->value().front()].emplace(
							nextAtom, item.originSymbol()
						);
					}
				}

				for(auto& trans : nextStates) {
					const auto newState = putState(std::move(trans.second));
					m_states[id].addTransition(newState, trans.first);
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