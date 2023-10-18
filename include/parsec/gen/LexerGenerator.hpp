#ifndef PARSEC_GEN_LEXER_GENERATOR_HEADER
#define PARSEC_GEN_LEXER_GENERATOR_HEADER

#include "../fg/Grammar.hpp"

#include <unordered_set>
#include <unordered_map>
#include <queue>
#include <map>

namespace parsec::gen {
	/**
	 * @brief Represents a single DFA state of a lexer.
	 */
	class LexerState {
	public:
		/**
		 * @brief Defines a DFA state change.
		 */
		class Transition {
		public:
			/** @{ */
			/** @brief Define a new state transition to another state. */
			Transition(int targetState, char label) noexcept
				: m_targetState(targetState), m_label(label)
			{ }
			/** @} */

			/** @{ */
			Transition(const Transition&) = default;
			Transition& operator=(const Transition&) = default;
			/** @} */

			/** @{ */
			/** @brief Identifier of the target state of the transition. */
			int targetState() const noexcept {
				return m_targetState;
			}

			/** @brief The condition under which the transition is performed. */
			char label() const noexcept {
				return m_label;
			}
			/** @} */

		private:
			int m_targetState = { };
			char m_label = { };
		};

		/** @brief List of @ref Transition "Transitions". */
		using TransitionList = std::vector<Transition>;


		/** @{ */
		/** @brief Construct an empty state. */
		LexerState() = default;

		/** @brief Construct a new state with an ID and an optional match. */
		LexerState(int id, const fg::TokenRule* match) noexcept
			: m_match(match), m_id(id)
		{ }
		/** @} */


		/** @{ */
		LexerState(const LexerState&) = default;
		LexerState& operator=(const LexerState&) = default;
		/** @} */

		/** @{ */
		LexerState(LexerState&&) = default;
		LexerState& operator=(LexerState&&) = default;
		/** @} */


		/** @{ */
		/** @brief List of allowed transitions for the state. */
		const TransitionList& transitions() const noexcept {
			return m_transitions;
		}

		/** @brief Check if the state is an accepting state. */
		bool accepting() const noexcept {
			return m_match != nullptr;
		}

		/** @brief A @ref fg::TokenRule "TokenRule", if any, that matches the analyzed input. */
		const fg::TokenRule* match() const noexcept {
			return m_match;
		}

		/** @brief Unique identifier for the state. */
		int id() const noexcept {
			return m_id;
		}
		/** @} */


		/** @{ */
		/** @brief Add a transition to the state. */
		void addTransition(int targetState, char label) {
			m_transitions.emplace_back(targetState, label);
		}
		/** @} */

	private:
		TransitionList m_transitions;
		const fg::TokenRule* m_match = nullptr;
		int m_id = { };
	};



	/**
	 * @brief Generates lexical analyzers from language grammars.
	 */
	class LexerGenerator {
	public:		
		/** @{ */
		/** @brief Generate a lexer for a language described by the grammar. */
		void compile(const fg::Grammar& grammar);
		/** @} */

	protected:
		/** @{ */
		LexerGenerator() = default;
		~LexerGenerator() = default;
		/** @} */

		/** @{ */
		LexerGenerator(LexerGenerator&&) = default;
		LexerGenerator& operator=(LexerGenerator&&) = default;
		/** @} */

		/** @{ */
		LexerGenerator(const LexerGenerator&) = delete;
		LexerGenerator& operator=(const LexerGenerator&) = delete;
		/** @} */


		/** @{ */
		/** @brief Called once for every generated LexerState. */
		virtual void onLexerState(const LexerState& state) { }

		/** @brief Called when a grammar is ready to be compiled. */
		virtual void onCompile() = 0;
		/** @} */


		/** @{ */
		/** @brief Input grammar for the compilation. */
		const fg::Grammar& inputGrammar() const noexcept {
			return *m_grammar;
		}

		/** @brief Generate @ref LexerState "LexerStates" for the target lexer. */
		void generateStates();
		/** @} */

	private:
		/** @{ */
		using State = std::unordered_set<const regex::CharLiteral*>;

		struct StateHasher {
			std::size_t operator()(const State& state) const;
		};

		using StateCache = std::unordered_map<State, int, StateHasher>;
		using StateHandle = const StateCache::value_type*;

		using TransitionMap = std::map<char, State>;
		/** @} */
		
		/** @{ */
		bool isEndMarker(const regex::CharLiteral* ch) const {
			return m_endMarkers.contains(ch);
		}
		
		const fg::TokenRule* computeRuleMatch(StateHandle state) const;
		TransitionMap computeTransitions(StateHandle state) const;

		State createStartState() const;
		void registerEndMarkers();

		void processState(StateHandle state);
		StateHandle cacheState(State&& state);
		/** @} */

		const fg::Grammar* m_grammar = nullptr;

		std::map<const regex::CharLiteral*, const fg::TokenRule*> m_endMarkers;
		std::queue<StateHandle> m_unprocessedStates;
		
		StateCache m_dfaStates;
		LexerState m_lexerState;
	};
}

#endif