#include "fsm/AutomatonFactory.hpp"
#include "fsm/AutomatonBuilder.hpp"

#include <boost/functional/hash.hpp>

#include <gsl/narrow>

#include <unordered_set>
#include <unordered_map>
#include <map>

namespace parsec::fsm {
	namespace {
		class Item {
		public:

			friend bool operator==(const Item& lhs, const Item& rhs) {
				return lhs.pattern()->id() == rhs.pattern()->id()
					&& lhs.m_pos == rhs.m_pos;
			}

			friend std::size_t hash_value(const Item& item) {
				return boost::hash_value(std::tuple(item.pattern()->id(), item.m_pos));
			}


			Item(const fg::RegularPattern* pattern, gsl::index pos)
				: m_pattern(pattern), m_pos(pos)
			{ }


			std::vector<gsl::index> followPos() const {
				auto fp = m_pattern->followPos(m_pos);
				if(const auto lp = m_pattern->lastPos(); std::ranges::find(lp, m_pos) != lp.end()) {
					fp.push_back(m_pattern->size());
				}
				return fp;
			}

			const fg::RegularPattern* pattern() const {
				return m_pattern;
			}

			char charValue() const {
				return m_pattern->atomAt(m_pos)->value();
			}

			bool isAtEnd() const {
				return m_pos == m_pattern->size();
			}


		private:
			const fg::RegularPattern* m_pattern = {};
			gsl::index m_pos = {};
		};


		using ItemSet = std::unordered_set<Item, boost::hash<Item>>;


		class ConstructDfa {
		public:
			Automaton operator()(const fg::RegularGrammar& grammar) {
				// initial state consists of the first characters of each pattern
				ItemSet startItems;
				for(const auto& pat : grammar.patterns()) {
					for(const auto fp : pat.firstPos()) {
						startItems.emplace(&pat, fp);
					}
				}

				// no items, no start state, no states at all
				if(!startItems.empty()) {
					createState(std::move(startItems));
					m_dfaBuilder.startState(0);
				}

				return m_dfaBuilder.build();
			}

		private:
			int createState(ItemSet&& stateItems) {
				const auto [it, wasInserted] = m_states.emplace(
					std::move(stateItems),
					gsl::narrow_cast<int>(m_states.size())
				);
				auto& [items, stateId] = *it;

				if(wasInserted) {
					processState(items, stateId);
				}
				return stateId;
			}

			void processState(const ItemSet& items, int state) {
				// incrementally build up all states to which there is a transition
				std::map<char, ItemSet> transitions;
				for(const auto& item : items) {
					// items that are located at the rule end represent possible matches
					if(item.isAtEnd()) {
						const bool wasInserted = m_acceptStates.insert(state).second;
						if(!wasInserted) {
							throw std::runtime_error("conflicting regular expression patterns");
						}
						m_dfaBuilder.acceptState(state, item.pattern()->name());
						continue;
					}

					for(const auto& next : item.followPos()) {
						transitions[item.charValue()].emplace(item.pattern(), next);
					}
				}

				for(auto& [inputChar, stateItems] : transitions) {
					const auto dest = createState(std::move(stateItems));
					m_dfaBuilder.transition(
						std::string(1, inputChar),
						state,
						dest
					);
				}
			}

			std::unordered_map<ItemSet, int, boost::hash<ItemSet>> m_states;
			std::unordered_set<int> m_acceptStates;
			AutomatonBuilder m_dfaBuilder;
		};
	}


	Automaton AutomatonFactory::makeDfa(const fg::RegularGrammar& regLang) {
		return ConstructDfa()(regLang);
	}
}