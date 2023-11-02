#include "lr/StateGen.hpp"
#include "fg/rules.hpp"

#include <boost/functional/hash.hpp>
#include <gsl/narrow>

#include <unordered_set>
#include <unordered_map>
#include <stack>

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
				if(sym && sym != m_currentSymbol) {
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
				return lhs.currentAtom() == rhs.currentAtom();
			}

			friend std::size_t hash_value(const Item& item) {
				return boost::hash_value(item.currentAtom());
			}
			/** @} */


			/** @{ */
			Item(const fg::Atom* atom, const fg::Symbol* symbol, int pos) noexcept
				: m_atom(atom), m_symbol(symbol), m_pos(pos)
			{ }
			/** @} */


			/** @{ */
			const fg::Atom* currentAtom() const noexcept {
				return m_atom;
			}

			const fg::Symbol* originSymbol() const noexcept {
				return m_symbol;
			}
			
			int pos() const noexcept {
				return m_pos;
			}

			bool atEnd() const noexcept {
				return m_atom == m_symbol->ruleEnd();
			}
			/** @} */


		private:
			const fg::Atom* m_atom;
			const fg::Symbol* m_symbol;
			int m_pos;
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
				putState(createStartState(), nullptr);

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

					// skip undefined and terminal symbols, end markers
					const auto sym = m_grammar.lookupSymbol(item->currentAtom()->value());
					if(!sym || item->atEnd() || sym->terminal()) {
						continue;
					}

					// all atoms forming the beginning of some rule must also be processed
					for(const auto atom : sym->rule()->leadingAtoms()) {
						unexpanded.emplace(atom, sym, 0);
					}
				}

				return closure;
			}

			ItemSet createStartState() const {
				ItemSet items;

				// add every leading atom of every rule to the start state
				for(const auto sym : FindStartSymbols(m_grammar)()) {
					for(const auto atom : sym->rule()->leadingAtoms()) {
						items.emplace(atom, sym, 0);
					}
				}

				return items;
			}
			
			int putState(ItemSet&& stateItems, const fg::Symbol* symbol) {
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
						m_states[id].addReduction(item.originSymbol(), item.pos());
						continue;
					}

					// otherwise, construct the next state to transition to on the input symbol
					for(const auto atom : item.currentAtom()->nextAtoms()) {
						// just skip references to undefined symbols
						const auto sym = m_grammar.lookupSymbol(item.currentAtom()->value());
						if(!sym) {
							continue;
						}

						stateGoto[sym].emplace(
							atom, item.originSymbol(), item.pos() + 1
						);
					}
				}

				// add shift actions for all generated transitions
				for(auto& [symbol, items] : stateGoto) {
					const auto newState = putState(std::move(items), symbol);
					m_states[id].addShift(symbol, newState);
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