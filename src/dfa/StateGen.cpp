#include "dfa/StateGen.hpp"
#include "fg/Atom.hpp"

#include <boost/functional/hash.hpp>
#include <gsl/narrow>

#include <unordered_set>
#include <unordered_map>
#include <stack>
#include <map>

namespace parsec::dfa {
	namespace {
		struct Item {
			/** @{ */
			friend bool operator==(const Item& lhs, const Item& rhs) noexcept {
				return lhs.atom() == rhs.atom();
			}

			friend std::size_t hash_value(const Item& item) {
				return boost::hash_value(item.atom());
			}
			/** @} */


			/** @{ */
			Item(const fg::Atom* atom, const fg::Symbol* symbol) noexcept
				: m_symbol(symbol), m_atom(atom)
			{ }
			/** @} */


			/** @{ */
			const fg::Atom* atom() const noexcept {
				return m_atom;
			}

			const fg::Symbol* symbol() const noexcept {
				return m_symbol;
			}

			bool atEnd() const noexcept {
				return m_symbol->rule()->endAtom() == m_atom;
			}
			/** @} */


		private:
			const fg::Symbol* m_symbol;
			const fg::Atom* m_atom;
		};

		using ItemSet = std::unordered_set<Item, boost::hash<Item>>;


		class RunImpl {
		public:
			explicit RunImpl(const fg::Grammar& grammar)
				: m_grammar(grammar)
			{ }
			
			StateList operator()() {
				// initial state consists of the first characters of each rule
				ItemSet startItems;
				for(const auto tok : m_grammar.terminals()) {
					for(const auto atom : tok->rule()->leadingAtoms()) {
						startItems.emplace(atom, tok);
					}
				}

				// no items, no start state, no states at all
				if(!startItems.empty()) {
					createState(std::move(startItems));

					while(!m_unprocessed.empty()) {
						const auto& [items, id] = *m_unprocessed.top();
						m_unprocessed.pop();

						processState(
							items,
							id
						);
					}
				}

				return m_states;
			}

		private:
			int createState(ItemSet&& stateItems) {
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
				// use map to sort transitions alphabetically
				std::map<char, ItemSet> nextStates;
				
				// incrementally build up all states to which there is a transition
				for(const auto& item : items) {
					// items that are located at the rule end represent possible matches
					if(item.atEnd()) {
						m_states[id].addMatch(item.symbol());
						continue;
					}

					for(const auto nextAtom : item.atom()->nextAtoms()) {
						// just skip ill-defined atoms
						if(item.atom()->value().size() != 1) {
							continue;
						}

						const auto ch = item.atom()->value().front();
						nextStates[ch].emplace(
							nextAtom,
							item.symbol()
						);
					}
				}

				// store the computed transitions in a State object
				for(auto& [label, stateItems] : nextStates) {
					const auto nextStateId = createState(std::move(stateItems));
					m_states[id].addTransition(
						nextStateId,
						label
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