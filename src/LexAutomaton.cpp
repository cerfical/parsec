#include "LexAutomaton.hpp"

#include "RegExParser.hpp"
#include "RegExNodes.hpp"

#include <boost/functional/hash.hpp>

#include <unordered_map>
#include <unordered_set>
#include <iomanip>
#include <ranges>

namespace {
	using namespace parsec;

	using RegExCharSet = std::unordered_set<const RegExChar*>;

	class Builder {
		using StateCache = std::unordered_map<RegExCharSet, LexAutomaton::State*, boost::hash<RegExCharSet>>;
		using State = StateCache::value_type;

	public:
		/** @{ */
		Builder(LexAutomaton& automaton, const LexTokenList& tokens)
		 : tokens(JoinTokens(tokens)), automaton(automaton)
		{ }
		/** @} */

		/** @{ */
		void Build() {
			const auto firstPos = tokens->GetFirstPos();
			const auto& startState = CacheState(
				{ firstPos.cbegin(), firstPos.cend() }
			);
			automaton.SetStartState(*startState.second);
		}
		/** @} */

	private:
		/** @{ */
		auto ComputeFollowPositions(const State& state) {
			std::unordered_map<char, RegExCharSet> followPositions;
			for(const auto ch : state.first) {
				if(!ProcessEndMarker(state, ch)) {
					const auto followPos = ch->GetFollowPos();
					followPositions[ch->GetValue()].insert(
						followPos.cbegin(), followPos.cend()
					);
				}
			}
			return followPositions;
		}

		void ProcessState(const State& state) {
			auto followPositions = ComputeFollowPositions(state);
			for(auto& followPos : followPositions) {
				const auto& newState = CacheState(std::move(followPos.second));
				state.second->AddTransition(*newState.second, followPos.first);
			}
		}

		const State& CacheState(RegExCharSet&& chars) {
			const auto state = states.try_emplace(std::move(chars), nullptr);
			if(state.second) {
				state.first->second = &automaton.AllocState();
				ProcessState(*state.first);
			}
			return *state.first;
		}
		
		bool ProcessEndMarker(const State& state, const RegExChar* ch) {
			const auto it = endMarkers.find(ch);
			if(it != endMarkers.cend()) {
				state.second->AddToken(*it->second);
				return true;
			}
			return false;
		}
		/** @} */

		/** @{ */
		std::unique_ptr<RegExNode> JoinTokens(const LexTokenList& tokens) {
			// parse all tokens and combine the resulting regular expressions
			auto regex = ParseToken(tokens.front());
			for(const auto& tok : tokens | std::ranges::views::drop(1)) {
				auto tokRegex = ParseToken(tok);
				regex = std::make_unique<RegExAltern>(
					std::move(regex), std::move(tokRegex)
				);
			}
			return std::move(regex);
		}

		std::unique_ptr<RegExNode> ParseToken(const LexToken& tok) {
			auto regex = RegExParser().Parse(tok.GetRegex());
			auto endMarker = NewEndMarker(tok);
			
			return std::make_unique<RegExConcat>(
				std::move(regex), std::move(endMarker)
			);
		}

		std::unique_ptr<RegExNode> NewEndMarker(const LexToken& tok) {
			// allocate a new node for the endmarker
			auto endMarker = std::make_unique<RegExChar>('#');
			endMarkers.emplace(endMarker.get(), &tok);
			return std::move(endMarker);
		}
		/** @} */

		std::unordered_map<const RegExChar*, const LexToken*> endMarkers;
		StateCache states;

		std::unique_ptr<RegExNode> tokens;
		LexAutomaton& automaton;
	};
}

namespace parsec {
	LexAutomaton::State LexAutomaton::emptyState;

	LexAutomaton::LexAutomaton(const LexGrammar& grammar) {
		Builder(*this, grammar.GetTokens()).Build();
	}

	void LexAutomaton::Dump(std::size_t indent, std::ostream& out) {
		const auto MakeIndent = [&](char delim = '-') {
			if(indent > 0) {
				return ' ' + std::string(indent, delim) + ' ';
			}
			return std::string();
		};

		const auto PrintState = [&, this](const State& state) {
			const auto IsStartState = [&]() { return &state == &GetStartState(); };

			const auto PrintHeading = [&] {
				out << MakeIndent() << "State #" << state.GetID();
				if(IsStartState() || state.IsAccepting()) {
					out << '(';
					if(IsStartState()) {
						out << 'S';
					} else {
						out << 'A';
					}
					out << ')';
				}
				out << ":\n";
			};

			const auto PrintPatterns = [&]() {
				const auto PrintToken = [&](const LexToken& tok) {
					out << tok.GetName() << ' ' << tok.GetRegex();
				};

				const auto patterns = MakeIndent(' ') + "  Patterns: ";
				const auto alignOff = state.GetMatchedTokens().front().GetName().size();
				out << patterns;
				PrintToken(state.GetMatchedTokens().front());
				out << '\n';
				for(const auto& tok : state.GetMatchedTokens() | std::ranges::views::drop(1)) {
					out << std::right << std::setw(patterns.size() + alignOff);
					PrintToken(tok);
					out << '\n';
				}
			};

			const auto PrintTransitions = [&]() {
				const auto PrintTrans = [&](const LexAutomaton::Transition& trans) {
					out << trans.GetLabel() << " => " << trans.GetNewState().GetID();
				};

				const auto transHead = MakeIndent(' ') + "  Trans: ";
				const auto alignOff = 1;
				out << transHead;
				PrintTrans(state.GetTransitions().front());
				out << '\n';
				for(const auto& trans : state.GetTransitions() | std::ranges::views::drop(1)) {
					out << std::right << std::setw(transHead.size() + alignOff);
					PrintTrans(trans);
					out << '\n';
				}
			};

			PrintHeading();
			if(state.IsAccepting()) {
				PrintPatterns();
			}
			if(state.GetTransitionsCount() > 0) {
				PrintTransitions();
			}
		};

		if(GetStatesCount() > 0) {
			PrintState(GetStates().front());
			for(const auto& state : GetStates() | std::ranges::views::drop(1)) {
				PrintState(state);
			}
		} else {
			out << MakeIndent() << "No states\n";
		}
	}
}