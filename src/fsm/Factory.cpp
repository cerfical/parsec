#include "fsm/Factory.hpp"

#include <boost/functional/hash.hpp>
#include <unordered_set>
#include <stack>

template <>
struct boost::hash<parsec::fg::Symbol> {
	std::size_t operator()(const parsec::fg::Symbol& symbol) {
		return std::hash<parsec::fg::Symbol>()(symbol);
	}
};

namespace parsec::fsm {
	namespace {
		class Item {
		public:

			Item(const fg::SymbolRule& rule, const fg::RegularExpr::Atom& atom)
				: m_rule(rule), m_atom(atom) {}
		

			const fg::SymbolRule& rule() const {
				return m_rule;
			}

			const fg::RegularExpr::Atom& atom() const {
				return m_atom;
			}


			bool isAtEnd() const {
				return m_atom.isEnd();
			}


		private:
			fg::RegularExpr::Atom m_atom;
			const fg::SymbolRule& m_rule;
		};

		using ItemSet = std::unordered_set<Item, boost::hash<Item>>;



		inline bool operator==(const Item& lhs, const Item& rhs) {
			return std::tuple(lhs.rule().head(), lhs.atom().posIndex()) == std::tuple(rhs.rule().head(), rhs.atom().posIndex());
		}

		inline std::size_t hash_value(const Item& item) {
			return boost::hash_value(std::tuple(item.rule().head(), item.atom().posIndex()));
		}



		class MakeFsm {
		public:

			StateMachine operator()(const fg::SymbolGrammar& grammar) {
				m_grammar = &grammar;

				if(auto startState = createStartState(); !startState.empty()) {
					createState(std::move(startState));
					m_fsm.setStartState(0);
				}

				return std::move(m_fsm);
			}

		protected:
			const fg::SymbolGrammar& inputGrammar() const {
				return *m_grammar;
			}

		private:
			virtual const ItemSet& computeClosure(const ItemSet& items) const = 0;

			virtual ItemSet createStartState() const = 0;


			int createState(ItemSet&& stateItems) {
				const auto [it, wasInserted] = m_states.emplace(std::move(stateItems), static_cast<int>(m_states.size()));
				const auto& [items, state] = *it;

				if(wasInserted) {
					buildStateTrans(items, state);
				}
				return state;
			}

			void buildStateTrans(const ItemSet& items, int state) {
				std::unordered_map<fg::Symbol, ItemSet> trans;
				for(const auto& item : computeClosure(items)) {
					if(item.isAtEnd()) {
						if(!m_fsm.stateById(state).inputMatch()) {
							m_fsm.markAcceptState(state, item.rule().head());
							continue;
						}
						throw std::runtime_error("conflicting rules");
					}

					auto& itemTrans = trans[item.atom().symbol()];
					for(const auto& pos : item.atom().followAtoms()) {
						itemTrans.emplace(item.rule(), pos);
					}
				}

				for(auto& [inputSymbol, stateItems] : trans) {
					const auto dest = createState(std::move(stateItems));
					m_fsm.buildTransition(state, dest, inputSymbol);
				}
			}


			std::unordered_map<ItemSet, int, boost::hash<ItemSet>> m_states;
			StateMachine m_fsm;

			const fg::SymbolGrammar* m_grammar = {};
		};



		class MakeDfa : public MakeFsm {
		private:
			const ItemSet& computeClosure(const ItemSet& items) const override {
				return items;
			}

			ItemSet createStartState() const override {
				ItemSet items;
				for(const auto& rule : inputGrammar().rules()) {
					for(const auto& pos : rule.body().firstAtoms()) {
						items.emplace(rule, pos);
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

					if(!wasInserted) {
						continue;
					}

					// expand only nonterminal symbols
					const auto& rule = inputGrammar().resolve(item->atom().symbol());
					for(const auto& pos : rule.body().firstAtoms()) {
						unexpanded.emplace(rule, pos);
					}
				}

				return m_expandedItems;
			}

			ItemSet createStartState() const override {
				ItemSet items;
				for(const auto& root = inputGrammar().root(); const auto& pos : root.body().firstAtoms()) {
					items.emplace(root, pos);
				}
				return items;
			}

			mutable ItemSet m_expandedItems;
		};
	}


	StateMachine Factory::makeDfa(const fg::SymbolGrammar& regLang) {
		return MakeDfa()(regLang);
	}

	StateMachine Factory::makeSlr(const fg::SymbolGrammar& lang) {
		return MakeSlr()(lang);
	}


	Factory* Factory::get() {
		static Factory factory;
		return &factory;
	}
}