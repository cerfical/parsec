#include "dfa/Automaton.hpp"

#include <boost/functional/hash.hpp>
#include <unordered_set>
#include <map>

template <>
struct boost::hash<parsec::fg::Symbol> {
	std::size_t operator()(const parsec::fg::Symbol& symbol) {
		return std::hash<parsec::fg::Symbol>()(symbol);
	}
};

namespace parsec::dfa {
	namespace {
		class Item {
		public:

			Item(const fg::Symbol& head, const fg::RegularExpr::Atom& atom)
				: m_head(head), m_atom(atom) {}


			const fg::Symbol& head() const {
				return m_head;
			}

			const fg::RegularExpr::Atom& atom() const {
				return m_atom;
			}

			bool isAtEnd() const {
				return m_atom.isEnd();
			}


		private:
			fg::RegularExpr::Atom m_atom;
			fg::Symbol m_head;
		};

		using ItemSet = std::unordered_set<Item, boost::hash<Item>>;


		inline bool operator==(const Item& lhs, const Item& rhs) {
			return std::tuple(lhs.head(), lhs.atom().posIndex())
				== std::tuple(rhs.head(), rhs.atom().posIndex());
		}

		inline std::size_t hash_value(const Item& item) {
			return boost::hash_value(std::tuple(item.head(), item.atom().posIndex()));
		}


		const State emptyState;
	}


	class Automaton::StateBuilder {
	public:

		StateBuilder(const fg::SymbolGrammar& grammar, StateList& states)
			: m_grammar(grammar), m_states(states) {}

		void run() {
			if(auto startState = createStartState(); !startState.empty()) {
				const auto startStateId = buildState(std::move(startState));
				m_states[startStateId].setStartState(true);
			}
		}

	private:
		ItemSet createStartState() const {
			ItemSet items;
			for(const auto& rule : m_grammar.rules()) {
				for(const auto& atom : rule.body().firstAtoms()) {
					items.emplace(rule.head(), atom);
				}
			}
			return items;
		}

		int buildState(ItemSet&& stateItems) {
			const auto [it, wasInserted] = m_itemSetsToIds.emplace(
				std::move(stateItems),
				static_cast<int>(m_itemSetsToIds.size())
			);
			const auto& [itemSet, stateId] = *it;

			if(wasInserted) {
				m_states.emplace_back(stateId);
				buildTransitions(itemSet, stateId);
			}
			return stateId;
		}

		void buildTransitions(const ItemSet& stateItems, int stateId) {
			// use map to lexicographically sort transitions by their labels
			std::map<fg::Symbol, ItemSet> transitions;
			
			for(const auto& item : stateItems) {
				if(item.isAtEnd()) {
					if(!m_states[stateId].isMatchState()) {
						m_states[stateId].setMatchedRule(item.head());
						continue;
					}
					throw std::runtime_error("conflicting rules");
				}

				auto& itemTrans = transitions[item.atom().symbol()];
				for(const auto& pos : item.atom().followAtoms()) {
					itemTrans.emplace(item.head(), pos);
				}
			}

			for(auto& [transLabel, transTarget] : transitions) {
				const auto transTargetId = buildState(std::move(transTarget));
				m_states[stateId].addTransition(transTargetId, transLabel);
			}
		}

		std::unordered_map<ItemSet, int, boost::hash<ItemSet>> m_itemSetsToIds;

		const fg::SymbolGrammar& m_grammar;
		StateList& m_states;
	};


	Automaton::Automaton(const fg::SymbolGrammar& grammar) {
		StateBuilder(grammar, m_states).run();
	}


	const State& Automaton::stateById(int state) const {
		if(state < m_states.size()) {
			return m_states[state];
		}
		return emptyState;
	}


	const State& Automaton::startState() const {
		if(!m_states.empty()) {
			return m_states.front();
		}
		return emptyState;
	}
}