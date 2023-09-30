#ifndef PARSEC_LEX_GENERATOR_HEADER
#define PARSEC_LEX_GENERATOR_HEADER

#include "LexPattern.hpp"

#include <unordered_set>
#include <unordered_map>
#include <queue>
#include <map>

namespace parsec {
	/** @brief Simple integer type for representing lexer states. */
	using LexState = std::size_t;

	/**
	 * @brief Generates a lexical analyzer from a list of recognizable token patterns.
	 */
	class LexGenerator {
	public:
		/** @{ */
		/** @copybrief */
		virtual ~LexGenerator() = default;
		/** @} */

		/** @{ */
		/** @brief Start the generation process. */
		virtual void Generate();
		/** @} */

	protected:
		/** @{ */
		/** @copybrief */
		LexGenerator() = default;

		/** @brief Set up a new LexGenerator. */
		LexGenerator(const LexPatternList* patterns)
		 : patterns(patterns)
		{ }
		/** @} */


		/** @{ */
		LexGenerator(LexGenerator&&) = default;
		LexGenerator& operator=(LexGenerator&&) = default;
		/** @} */


		/** @{ */
		LexGenerator(const LexGenerator&) = delete;
		LexGenerator& operator=(const LexGenerator&) = delete;
		/** @} */


		/** @{ */
		/** @brief Called before processing of the state begins. */
		virtual void OnPreState(LexState state) = 0;


		/** @brief Called before processing of the state transitions begins. */
		virtual void OnPreTransitions() = 0;
		
		/** @brief Called once for every state transition. */
		virtual void OnTransition(LexState state, char label) = 0;

		/** @brief Called after processing of the state transitions is complete. */
		virtual void OnPostTransitions() = 0;
		

		/** @brief Called when the state should report a successful match. */
		virtual void OnAccept(const LexPattern& match) = 0;

		/** @brief Called when the state should report a malformed input. */
		virtual void OnReject() = 0;


		/** @brief Called after processing of the state is complete. */
		virtual void OnPostState(LexState state) = 0;
		/** @} */


		/** @{ */
		/** @brief Get the input list of patterns. */
		const LexPatternList* GetPatterns() const noexcept {
			return patterns;
		}
		/** @} */

	private:
		/** @{ */
		using DfaState = std::unordered_set<const RegExChar*>;

		struct DfaStateHasher {
			std::size_t operator()(const DfaState& state) const;
		};

		using StateCache = std::unordered_map<DfaState, LexState, DfaStateHasher>;
		using StateHandle = const StateCache::value_type*;

		using TransitionMap = std::map<char, DfaState>;
		/** @} */
		
		/** @{ */
		bool IsEndMarker(const RegExChar* ch) const {
			return endMarkers.contains(ch);
		}

		void RegisterEndMarkers();
		void ComputeMatches(StateHandle state);
		
		TransitionMap ComputeNextStates(StateHandle state);
		DfaState CreateStartState();

		void ProcessState(StateHandle state);
		StateHandle CacheState(DfaState&& state);
		/** @} */

		const LexPatternList* patterns = nullptr;
		StateCache states;

		std::map<const RegExChar*, const LexPattern*> endMarkers;
		std::queue<StateHandle> unprocessedStates;
	};
}

#endif