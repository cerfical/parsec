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


		const State emptyState(0);
	}


	class Automaton::Builder {
	public:

		Builder(Automaton& dfa)
			: m_dfa(dfa) {}

		void buildFromGrammar(const fg::SymbolGrammar& inputGrammar) {
			if(auto startState = createStartState(inputGrammar); !startState.empty()) {
				createState(std::move(startState));
				m_dfa.m_states[0].setStartState(true);
			}
		}

	private:
		ItemSet createStartState(const fg::SymbolGrammar& inputGrammar) const {
			ItemSet items;
			for(const auto& rule : inputGrammar.rules()) {
				for(const auto& atom : rule.body().firstAtoms()) {
					items.emplace(rule.head(), atom);
				}
			}
			return items;
		}

		int createState(ItemSet&& stateItems) {
			const auto [itemsToIdIt, wasInserted] = m_itemsToId.emplace(
				std::move(stateItems),
				static_cast<int>(m_itemsToId.size())
			);

			if(wasInserted) {
				m_dfa.m_states.emplace_back(itemsToIdIt->second);
				buildStateTrans(itemsToIdIt->first, itemsToIdIt->second);
			}
			return itemsToIdIt->second;
		}

		void buildStateTrans(const ItemSet& items, int state) {
			std::map<fg::Symbol, ItemSet> trans;
			for(const auto& item : items) {
				if(item.isAtEnd()) {
					if(!m_dfa.stateById(state).isAcceptState()) {
						m_dfa.m_states[state].setAcceptSymbol(item.head());
						continue;
					}
					throw std::runtime_error("conflicting rules");
				}

				auto& itemTrans = trans[item.atom().symbol()];
				for(const auto& pos : item.atom().followAtoms()) {
					itemTrans.emplace(item.head(), pos);
				}
			}

			for(auto& [inputSymbol, stateItems] : trans) {
				const auto dest = createState(std::move(stateItems));
				m_dfa.m_states[state].addTransition(dest, inputSymbol);
			}
		}

		std::unordered_map<ItemSet, int, boost::hash<ItemSet>> m_itemsToId;
		Automaton& m_dfa;
	};


	Automaton::Automaton(const fg::SymbolGrammar& inputGrammar) {
		Builder(*this).buildFromGrammar(inputGrammar);
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