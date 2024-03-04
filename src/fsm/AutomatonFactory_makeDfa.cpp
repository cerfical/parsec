#include "fsm/AutomatonFactory.hpp"
#include "fsm/AutomatonBuilder.hpp"

#include <boost/functional/hash.hpp>

#include <unordered_set>
#include <unordered_map>

namespace parsec::fsm {
	namespace {
		class Item {
		public:

			Item(const fg::RegularPattern* pattern, std::size_t pos) noexcept
				: m_pattern(pattern), m_pos(pos) {
			}


			const fg::RegularPattern* pattern() const noexcept {
				return m_pattern;
			}


			char charValue() const noexcept {
				return m_pattern->charAt(m_pos).value();
			}

			std::size_t pos() const noexcept {
				return m_pos;
			}

			bool isAtEnd() const noexcept {
				return !m_pattern->charAt(m_pos).has_value();
			}


		private:
			const fg::RegularPattern* m_pattern = {};
			std::size_t m_pos = {};
		};

		using ItemSet = std::unordered_set<Item, boost::hash<Item>>;



		inline bool operator==(const Item& lhs, const Item& rhs) noexcept {
			return lhs.pattern()->id() == rhs.pattern()->id() && lhs.pos() == rhs.pos();
		}

		inline std::size_t hash_value(const Item& item) noexcept {
			return boost::hash_value(std::tuple(item.pattern()->id(), item.pos()));
		}



		class ConstructDfa {
		public:

			Automaton operator()(const fg::RegularGrammar& grammar) {
				ItemSet startItems;
				for(const auto& pat : grammar.patterns()) {
					for(const auto pos : pat.firstPos()) {
						startItems.emplace(&pat, pos);
					}
				}

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
					static_cast<int>(m_states.size())
				);
				auto& [items, state] = *it;

				if(wasInserted) {
					buildStateTrans(items, state);
				}
				return state;
			}

			void buildStateTrans(const ItemSet& items, int state) {
				std::unordered_map<char, ItemSet> trans;
				for(const auto& item : items) {
					if(item.isAtEnd()) {
						const bool wasInserted = m_acceptStates.insert(state).second;
						if(!wasInserted) {
							throw std::runtime_error("conflicting regular expression patterns");
						}
						m_dfaBuilder.acceptState(state, item.pattern()->name());
						continue;
					}

					auto& stateTrans = trans[item.charValue()];
					for(const auto& pos : item.pattern()->followPos(item.pos())) {
						stateTrans.emplace(item.pattern(), pos);
					}
				}

				for(auto& [inputChar, stateItems] : trans) {
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