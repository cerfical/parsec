#pragma once

#include "StateTrans.hpp"
#include <vector>

namespace parsec::fsm {

    /**
     * @brief Represents a single state in a DfaAutomaton.
     */
    class DfaState {
    public:

        DfaState() = default;

        DfaState(const DfaState& other) = default;
        DfaState& operator=(const DfaState& other) = default;

        DfaState(DfaState&& other) noexcept = default;
        DfaState& operator=(DfaState&& other) noexcept = default;

        ~DfaState() = default;


        /** @{ */
        explicit DfaState(int id)
            : id_(id) {}


        /**
         * @brief List of transitions from the state.
         */
        const std::vector<StateTrans>& transitions() const noexcept {
            return transitions_;
        }


        /**
         * @brief Add a transition to the state.
         */
        void addTransition(int target, const Symbol& label) {
            transitions_.emplace_back(target, label);
        }


        /**
         * @brief Symbolic match produced by the state.
         */
        const Symbol& match() const noexcept {
            return match_;
        }


        /**
         * @brief Set the state match.
         */
        void setMatch(const Symbol& match) {
            match_ = match;
        }


        /**
         * @brief Check if the state is the start state.
         */
        bool isStartState() const noexcept {
            return id() == 0;
        }


        /**
         * @brief Check if the state is a match state.
         */
        bool isMatchState() const noexcept {
            return !match().isEmpty();
        }


        /**
         * @brief Unique identifier of the state in the automaton.
         */
        int id() const noexcept {
            return id_;
        }
        /** @} */


    private:
        std::vector<StateTrans> transitions_;
        Symbol match_;

        int id_ = {};
    };

}
