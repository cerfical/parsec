#include "lr/StateGen.hpp"

#include <boost/functional/hash.hpp>
#include <gsl/narrow>

#include <unordered_set>
#include <unordered_map>
#include <stack>

namespace parsec::lr {
	namespace {
		struct Item {
			friend bool operator==(const Item& lhs, const Item& rhs) noexcept {
				return lhs.rule()->id() == rhs.rule()->id() && lhs.pos() == rhs.pos();
			}

			friend std::size_t hash_value(const Item& item) {
				std::size_t seed = 0;

				boost::hash_combine(seed, boost::hash_value(item.rule()->id()));
				boost::hash_combine(seed, boost::hash_value(item.pos()));
				
				return seed;
			}


			Item(
				const Rule* rule,
				int pos = 0,
				int tokens = 0
			) noexcept
				: m_rule(rule)
				, m_tokens(tokens)
				, m_pos(pos)
			{ }


			const Symbol* symbol() const noexcept {
				return m_rule->symbolAt(m_pos);
			}

			int tokens() const noexcept {
				return m_tokens;
			}
			
			const Rule* rule() const noexcept {
				return m_rule;
			}

			int pos() const noexcept {
				return m_pos;
			}

			bool isAtEnd() const noexcept {
				return m_rule->length() == m_pos;
			}

			Item next() const noexcept {
				return Item(
					m_rule,
					m_pos + 1,
					m_tokens + (symbol()->pattern() && !symbol()->isReserved() ? 1 : 0)
				);
			}


		private:
			const Rule* m_rule;

			int m_tokens;
			int m_pos;
		};

		using ItemSet = std::unordered_set<Item, boost::hash<Item>>;


		class RunImpl {
		public:
			explicit RunImpl(const Grammar& grammar)
				: m_grammar(grammar)
			{ }

			auto operator()() {
				// construct the initial state from the first symbols of each rule
				ItemSet startItems;
				for(const auto rule : m_grammar.root()->rules()) {
					startItems.emplace(rule);
				}

				// allocate states only if there is something to construct them from
				if(!startItems.empty()) {
					createState(std::move(startItems), nullptr);
					while(!m_unprocessed.empty()) {
						const auto& [items, id] = *m_unprocessed.top();
						m_unprocessed.pop();

						processState(items, id);
					}
				}

				return std::move(m_states);
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
					const auto [item, wasInserted] = closure.emplace(unexpanded.top());
					if(unexpanded.pop(); !wasInserted || item->isAtEnd()) {
						continue;
					}

					for(const auto rule : item->symbol()->rules()) {
						unexpanded.emplace(rule);
					}
				}

				return closure;
			}

			int createState(ItemSet&& stateItems, const Symbol* symbol) {
				const auto newId = gsl::narrow_cast<int>(m_states.size()); // unique identifier for a new state
				const auto [it, wasInserted] = m_itemsToStates.emplace(std::move(stateItems), newId);
				const auto& [items, id] = *it;

				// construct a new state only if it differs from the previously created ones
				if(wasInserted) {
					m_states.emplace_back(newId, symbol);
					m_unprocessed.push(std::to_address(it));
				}

				return id;
			}

			void processState(const ItemSet& items, int id) {
				std::unordered_map<const Symbol*, ItemSet> transitions;
				for(const auto& item : computeClosure(items)) {
					// if we have reached the end of the rule, add a reduce action
					if(item.isAtEnd()) {
						m_states[id].addReduce(
							item.rule(),
							item.pos(),
							item.tokens()
						);
						continue;
					}

					// otherwise, construct the next state to transition to on the given symbol
					transitions[item.symbol()].insert(item.next());
				}

				// add shift and goto actions for all generated transitions
				for(auto& [symbol, items] : transitions) {
					const auto newState = createState(std::move(items), symbol);
					if(symbol->pattern() || symbol == m_grammar.eof()) {
						m_states[id].addShift(symbol, newState);
					} else {
						m_states[id].addGoto(symbol, newState);
					}
				}
			}


			std::unordered_map<ItemSet, int, boost::hash<ItemSet>> m_itemsToStates;
			std::stack<const std::pair<const ItemSet, int>*> m_unprocessed;
			
			const Grammar& m_grammar;
			std::vector<State> m_states;
		};
	}


	std::vector<State> StateGen::run(const Grammar& grammar) {
		return RunImpl(grammar)();
	}
}