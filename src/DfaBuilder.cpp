#include "DfaBuilder.hpp"

namespace {
	using namespace parsec;

	using Position = DfaStateMachine::Position;
	using PositionSet = DfaStateMachine::PositionSet;

	using State = DfaStateMachine::State;
	using StateList = DfaStateMachine::StateList;

	class RegExTables {
		class Builder : public RegExTraverser {
		public:
			/** @{ */
			Builder(RegExTables& tables) noexcept
			 : tables(tables)
			{ }
			/** @} */

			/** @{ */
			Uid GetEndMarkerID() const noexcept {
				return nextPosId;
			}
			/** @} */

		private:
			/** @{ */
			void Visit(const RegExChar& n) override {
				ComputeNullable(n);
				ComputeFirstPos(n);
				ComputeLastPos(n);
				ComputeFollowPos(n);
				nextPosId++;
			}

			void Visit(const RegExStar& n) override {
				ComputeNullable(n);
				ComputeFirstPos(n);
				ComputeLastPos(n);
				ComputeFollowPos(n);
			}
			
			void Visit(const RegExAltern& n) override {
				ComputeNullable(n);
				ComputeFirstPos(n);
				ComputeLastPos(n);
				ComputeFollowPos(n);
			}

			void Visit(const RegExConcat& n) override {
				ComputeNullable(n);
				ComputeFirstPos(n);
				ComputeLastPos(n);
				ComputeFollowPos(n);
			}

			TraversalTypes GetTraversalType() const noexcept override {	
				return TraversalTypes::Postorder;
			}
			/** @} */


			/** @{ */
			void ComputeNullable(const RegExAltern& n) {
				if(tables.IsNullable(n.GetLeftExpr()) || tables.IsNullable(n.GetRightExpr())) {
					tables.nullable.insert(&n);
				}
			}
			
			void ComputeFirstPos(const RegExAltern& n) {
				// add to the firstpos set of the alternation node the firstpos set of its left child
				const auto& left = tables.GetLeadingChars(n.GetLeftExpr());
				tables.firstpos[&n].insert(left.cbegin(), left.cend());
				
				// add the firstpos set of its right child
				const auto& right = tables.GetLeadingChars(n.GetRightExpr());
				tables.firstpos[&n].insert(right.cbegin(), right.cend());
			}

			void ComputeLastPos(const RegExAltern& n) {
				// add to the lastpos set of the alternation node the lastpos set of its left child
				const auto& left = tables.GetTrailingChars(n.GetLeftExpr());
				tables.lastpos[&n].insert(left.cbegin(), left.cend());

				// add the lastpos set of its right child
				const auto& right = tables.GetTrailingChars(n.GetRightExpr());
				tables.lastpos[&n].insert(right.cbegin(), right.cend());
			}

			void ComputeFollowPos(const RegExAltern& n) {
				// nothing to do here
			}
			/** @} */


			/** @{ */
			void ComputeNullable(const RegExConcat& n) {
				if(tables.IsNullable(n.GetLeftExpr()) && tables.IsNullable(n.GetRightExpr())) {
					tables.nullable.insert(&n);
				}
			}
			
			void ComputeFirstPos(const RegExConcat& n) {
				// add the firstpos set of the left subexpression
				const auto& left = tables.GetLeadingChars(n.GetLeftExpr());
				tables.firstpos[&n].insert(left.cbegin(), left.cend());

				// and if the left subexpression is nullable, also add the right
				if(tables.IsNullable(n.GetLeftExpr())) {
					const auto& right = tables.GetLeadingChars(n.GetRightExpr());
					tables.firstpos[&n].insert(right.cbegin(), right.cend());
				}
			}

			void ComputeLastPos(const RegExConcat& n) {
				// add the lastpos set of the right subexpression
				const auto& right = tables.GetTrailingChars(n.GetRightExpr());
				tables.lastpos[&n].insert(right.cbegin(), right.cend());
				
				// and if the right subexpression is nullable, also add the left
				if(tables.IsNullable(n.GetRightExpr())) {
					const auto& left = tables.GetTrailingChars(n.GetLeftExpr());
					tables.lastpos[&n].insert(left.cbegin(), left.cend());
				}
			}

			void ComputeFollowPos(const RegExConcat& n) {
				const auto& left = tables.GetTrailingChars(n.GetLeftExpr());
				const auto& right = tables.GetLeadingChars(n.GetRightExpr());
				// add the right's firstpos to the followpos of each position in the left's lastpos
				for(const auto& pos : left) {
					tables.followpos[pos].insert(right.cbegin(), right.cend());
				}
			}
			/** @} */

			
			/** @{ */
			void ComputeNullable(const RegExChar& n) {
				// nothing to do here
			}
			
			void ComputeFirstPos(const RegExChar& n) {
				tables.firstpos[&n].emplace(n.GetValue(), nextPosId);
			}

			void ComputeLastPos(const RegExChar& n) {
				tables.lastpos[&n].emplace(n.GetValue(), nextPosId);
			}

			void ComputeFollowPos(const RegExChar& n) {
				// nothing to do here
			}
			/** @} */


			/** @{ */
			void ComputeNullable(const RegExStar& n) {
				tables.nullable.insert(&n);
			}
			
			void ComputeFirstPos(const RegExStar& n) {
				const auto& inner = tables.GetLeadingChars(n.GetInnerExpr());
				tables.firstpos[&n].insert(inner.cbegin(), inner.cend());
			}

			void ComputeLastPos(const RegExStar& n) {
				const auto& inner = tables.GetTrailingChars(n.GetInnerExpr());
				tables.lastpos[&n].insert(inner.cbegin(), inner.cend());
			}

			void ComputeFollowPos(const RegExStar& n) {
				const auto& last = tables.GetTrailingChars(n.GetInnerExpr());
				const auto& first = tables.GetLeadingChars(n.GetInnerExpr());
				// add the inner's firstpos to the followpos of each position in the inner's lastpos
				for(const auto& pos : last) {
					tables.followpos[pos].insert(first.cbegin(), first.cend());
				}
			}
			/** @} */

			RegExTables& tables;
			Uid nextPosId = 0;
		};
		
	public:
		/** @{ */
		static RegExTables Compute(const RegExNode& regex) {
			RegExTables tables;

			Builder builder(tables);
			regex.Traverse(builder);
			
			// augment the followpos set of ending positions of the regex with end markers
			const auto& last = tables.GetTrailingChars(regex);
			for(const auto& pos : last) {
				tables.followpos[pos].emplace(builder.GetEndMarkerID());
			}

			return tables;
		}
		/** @} */

		/** @{ */
		const PositionSet& GetLeadingChars(const RegExNode& n) {
			return firstpos[&n];
		}
		const PositionSet& GetTrailingChars(const RegExNode& n) {
			return lastpos[&n];
		}
		const PositionSet& GetFollowChars(const Position& pos) {
			return followpos[pos];
		}

		bool IsNullable(const RegExNode& n) {
			return nullable.contains(&n);
		}
		/** @} */

	private:
		using NullableSet = std::unordered_set<const RegExNode*>;
		using FirstPosSet = std::unordered_map<const RegExNode*, PositionSet>;
		using LastPosSet = std::unordered_map<const RegExNode*, PositionSet>;
		using FollowPosSet = std::unordered_map<Position, PositionSet, PositionSet::hasher>;

		NullableSet nullable;
		FirstPosSet firstpos;
		LastPosSet lastpos;
		FollowPosSet followpos;
	};

	class DfaBuilderImpl  {
		/** @{ */
		template <typename PS>
		State CreateState(PS&& positions) {
			const auto prevStatesCount = dfa.GetStatesCount();
			const auto state = dfa.PutState(std::forward<PS>(positions));
			// add the state to the "unmarked" list only if it is "new"
			if(dfa.GetStatesCount() > prevStatesCount) {
				unmarkedStates.push_back(state);
			}
			return state;
		}

		State GetUnmarkedState() noexcept {
			const auto s = unmarkedStates.back();
			unmarkedStates.pop_back();
			return s;
		}

		bool AnyStatesUnmarked() const noexcept {
			return !unmarkedStates.empty();
		}

		auto ComputeFollowPos(const State& state) {
			std::unordered_map<char, PositionSet> followpos;
			for(const auto& pos : state.GetPositions()) {
				if(!pos.IsEndMarker()) {
					const auto& followChars = tables.GetFollowChars(pos);
					followpos[pos.GetChar()].insert(followChars.cbegin(), followChars.cend());
				} else {
					dfa.AddAcceptingState(state);
				}
			}
			return followpos;
		}
		/** @} */

	public:
		/** @{ */
		DfaBuilderImpl(const RegExNode& regex) 
		 : regex(regex) {
			tables = RegExTables::Compute(regex);
		}
		/** @} */

		/** @{ */
		DfaStateMachine Build() {
			const auto startState = CreateState(tables.GetLeadingChars(regex));
			while(AnyStatesUnmarked()) {
				const auto state = GetUnmarkedState();
				auto followpos = ComputeFollowPos(state);
				for(auto& pos : followpos) {
					const auto newState = CreateState(std::move(pos.second));
					dfa.AddTransition(state, newState, pos.first);
				}
			}
			dfa.SetStartState(startState);
			return std::move(dfa);
		}
		/** @} */

	private:
		const RegExNode& regex;

		StateList unmarkedStates;
		RegExTables tables;

		DfaStateMachine dfa;
	};
}

namespace parsec {
	DfaStateMachine DfaBuilder::Build() {
		return DfaBuilderImpl(*regex)
			.Build();
	}
}