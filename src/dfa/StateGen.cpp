#include "dfa/StateGen.hpp"

#include <boost/functional/hash.hpp>
#include <gsl/narrow>

#include <unordered_set>
#include <unordered_map>
#include <stack>
#include <map>

namespace parsec::dfa {
	namespace {
		struct Item {
			friend bool operator==(const Item& lhs, const Item& rhs) noexcept {
				return lhs.pattern()->id() == rhs.pattern()->id()
					&& lhs.atom()->id() == rhs.atom()->id();
			}

			friend std::size_t hash_value(const Item& item) {
				std::size_t seed = 0;

				boost::hash_combine(seed, item.pattern()->id());
				boost::hash_combine(seed, item.atom()->id());

				return seed;
			}


			Item(const CharAtom* atom, const Pattern* pattern) noexcept
				: m_pattern(pattern), m_atom(atom)
			{ }


			const CharAtom* atom() const noexcept {
				return m_atom;
			}

			const Pattern* pattern() const noexcept {
				return m_pattern;
			}

			bool isAtEnd() const noexcept {
				return m_pattern->endAtom() == m_atom;
			}


		private:
			const Pattern* m_pattern;
			const CharAtom* m_atom;
		};

		using ItemSet = std::unordered_set<Item, boost::hash<Item>>;


		class RunImpl {
		public:
			explicit RunImpl(const Grammar& grammar)
				: m_grammar(grammar)
			{ }
			
			auto operator()() {
				// initial state consists of the first characters of each pattern
				ItemSet startItems;
				for(const auto tok : m_grammar.tokenPatterns()) {
					for(const auto root : tok->roots()) {
						startItems.emplace(root, tok);
					}
				}

				// no items, no start state, no states at all
				if(!startItems.empty()) {
					createState(std::move(startItems));

					while(!m_unprocessed.empty()) {
						const auto& [items, id] = *m_unprocessed.top();
						m_unprocessed.pop();

						processState(items, id);
					}
				}

				return std::move(m_states);
			}

		private:
			int createState(ItemSet&& stateItems) {
				const auto newId = gsl::narrow_cast<int>(m_states.size()); // unique identifier for a new state
				const auto [it, wasInserted] = m_itemsToStates.emplace(std::move(stateItems), newId);
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
					if(item.isAtEnd()) {
						m_states[id].addMatch(item.pattern());
						continue;
					}

					for(const auto followAtom : item.atom()->follows()) {
						const auto ch = item.atom()->value();
						nextStates[ch].emplace(
							followAtom,
							item.pattern()
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


			std::unordered_map<ItemSet, int, boost::hash<ItemSet>> m_itemsToStates;
			std::stack<const std::pair<const ItemSet, int>*> m_unprocessed;
			
			std::vector<State> m_states;
			const Grammar& m_grammar;
		};
	}


	std::vector<State> StateGen::run(const Grammar& grammar) {
		return RunImpl(grammar)();
	}
}