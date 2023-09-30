#ifndef PARSEC_LEX_GEN_HEADER
#define PARSEC_LEX_GEN_HEADER

#include "Grammar.hpp"

#include <unordered_set>
#include <unordered_map>
#include <queue>
#include <map>

namespace parsec {
	/**
	 * @brief Generates lexical analyzers for language grammars.
	 */
	class LexGen {
	public:
		/** @{ */
		/** @brief Set up an empty lexer generator that does nothing. */
		LexGen() = default;

		/** @brief Set up a new lexer generator for a grammar. */
		explicit LexGen(const Grammar* grammar)
		 : m_grammar(grammar)
		{ }
		/** @} */

		/** @{ */
		LexGen(LexGen&&) = default;
		LexGen& operator=(LexGen&&) = default;
		/** @} */

		/** @{ */
		LexGen(const LexGen&) = delete;
		LexGen& operator=(const LexGen&) = delete;
		/** @} */
		
		/** @{ */
		/** @brief Start the generation process. */
		virtual void Gen();
		/** @} */

	protected:
		/** @brief Simple integer type for representing generated lexer states. */
		using StateID = int;

		/** @{ */
		~LexGen() = default;
		/** @} */


		/** @{ */
		/** @brief Grammar used as the input to the generator. */
		const Grammar* GetInputGrammar() const noexcept {
			return m_grammar;
		}
		/** @} */


		/** @{ */
		/** @brief Called before processing of the state begins. */
		virtual void OnPreState(StateID state) = 0;


		/** @brief Called before processing of the state transitions begins. */
		virtual void OnPreTransitions() = 0;
		
		/** @brief Called once for every state transition. */
		virtual void OnTransition(StateID state, char label) = 0;

		/** @brief Called after processing of the state transitions is complete. */
		virtual void OnPostTransitions() = 0;
		

		/** @brief Called when the state should report a successful match. */
		virtual void OnAccept(const TokenPattern& match) = 0;

		/** @brief Called when the state should report a malformed input. */
		virtual void OnReject() = 0;


		/** @brief Called after processing of the state is complete. */
		virtual void OnPostState(StateID state) = 0;
		/** @} */

	private:
		/** @{ */
		using State = std::unordered_set<const RegExChar*>;

		struct StateHasher {
			std::size_t operator()(const State& state) const;
		};

		using StateCache = std::unordered_map<State, StateID, StateHasher>;
		using StateHandle = const StateCache::value_type*;

		using TransitionMap = std::map<char, State>;
		/** @} */
		
		/** @{ */
		bool IsEndMarker(const RegExChar* ch) const {
			return m_endMarkers.contains(ch);
		}

		void RegisterEndMarkers();
		void ComputeMatches(StateHandle state);
		
		TransitionMap ComputeNextStates(StateHandle state);
		State CreateStartState();

		void ProcessState(StateHandle state);
		StateHandle CacheState(State&& state);
		/** @} */

		const Grammar* m_grammar = nullptr;
		StateCache m_states;

		std::map<const RegExChar*, const TokenPattern*> m_endMarkers;
		std::queue<StateHandle> m_unprocessedStates;
	};
}

#endif