#include "DfaStateMachine.hpp"
#include <boost/functional/hash.hpp>

#include <ranges>
#include <iomanip>

using namespace parsec;

using Position = DfaStateMachine::Position;
using PositionSet = DfaStateMachine::PositionSet;

using State = DfaStateMachine::State;
using StateTransition = DfaStateMachine::StateTransition;

namespace parsec {
	static std::size_t hash_value(const Position& pos) noexcept {
		std::size_t seed = 0;
		boost::hash_combine(seed, pos.GetID());
		if(!pos.IsEndMarker()) {
			boost::hash_combine(seed, pos.GetChar());
		}
		return seed;
	}
	DfaStateMachine::StateIdsMap::value_type State::emptyState;

	
	std::ostream& operator<<(std::ostream& out, const Position& pos) {
		out << pos.GetID();
		return out;
	}
	
	std::ostream& operator<<(std::ostream& out, const State& state) {
		out << state.GetID() << ": ( ";
		for(const auto& pos : state.GetPositions()) {
			out << pos << ' ';
		}
		out << ')';
		return out;
	}

	std::ostream& operator<<(std::ostream& out, const StateTransition& trans) {
		out << trans.GetFromState().GetID() << ", " << trans.GetLabel() << " => " << trans.GetToState().GetID();
		return out;
	}


	std::size_t DfaStateMachine::PositionHasher::operator()(const Position& pos) const noexcept {
		return hash_value(pos);
	}

	std::size_t DfaStateMachine::PositionSetHasher::operator()(const PositionSet& positions) const {
		return boost::hash_value(positions);
	}

	void DfaStateMachine::Dump(std::ostream& out) const {
		constexpr static auto GetNumberWidth = [](auto num) {
			return (std::ostringstream() << num).str().size();
		};

		if(GetStatesCount() > 0) {
			constexpr static std::string_view statesHeading = " - States: ";
			const auto PrintState = [&, this](const State& state) {
				const auto IsAcceptingState = [&, this]() {
					return std::ranges::find(GetAcceptingStates(), state) != GetAcceptingStates().cend();
				};
				
				const auto IsStartState = [&, this]() {
					return state == GetStartState();
				};

				out << state;
				if(IsStartState()) {
					out << " S";
				}
				if(IsAcceptingState()) {
					out << " A";
				}
				out << '\n';
			};
			
			out << statesHeading;
			PrintState(GetStates().front());
			const auto offset = GetNumberWidth(GetStates().front().GetID());
			for(const auto& state : GetStates() | std::ranges::views::drop(1)) {
				out << std::right << std::setw(statesHeading.size() + offset);
				PrintState(state);
			}
		} else {
			out << " - No states\n";
		}

		if(GetTransitionsCount() > 0) {
			constexpr static std::string_view transitionsHeading = " - Transitions: ";
			out << transitionsHeading << GetTransitions().front() << '\n';
			const auto offset = GetNumberWidth(GetTransitions().front().GetFromState().GetID());
			for(const auto& trans : GetTransitions() | std::ranges::views::drop(1)) {
				out << std::right << std::setw(transitionsHeading.size() + offset);
				out << trans << '\n';
			}
		} else {
			out << " - No transitions\n";
		}
	}
}