#pragma once

#include "StateTrans.hpp"
#include <vector>

namespace parsec::fsm {

    /**
     * @brief Represents a single state in a ElrAutomaton.
     */
    class ElrState {
    public:

        ElrState() = default;

        ElrState(const ElrState& other) = default;
        ElrState& operator=(const ElrState& other) = default;

        ElrState(ElrState&& other) noexcept = default;
        ElrState& operator=(ElrState&& other) noexcept = default;

        ~ElrState() = default;


        /** @{ */
        explicit ElrState(int id)
            : id_(id) {}


        /**
         * @brief List of shift transitions from the state.
         */
        const std::vector<StateTrans>& shifts() const noexcept {
            return shifts_;
        }


        /**
         * @brief Add a shift transition to the state.
         */
        void addShift(int target, const Symbol& label) {
            shifts_.emplace_back(target, label);
        }


        /**
         * @brief List of goto transitions from the state.
         */
        const std::vector<StateTrans>& gotos() const noexcept {
            return gotos_;
        }


        /**
         * @brief Add a goto transition to the state.
         */
        void addGoto(int target, const Symbol& label) {
            gotos_.emplace_back(target, label);
        }


        /**
         * @brief List of the state backlinks.
         */
        const std::vector<int>& backlinks() const noexcept {
            return backlinks_;
        }


        /**
         * @brief Add a backlink to the state.
         */
        void addBacklink(int backlink) {
            backlinks_.emplace_back(backlink);
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
         * @brief Backlink referencing a backlink of the parent state.
         */
        int backlink() const noexcept {
            return backlink_;
        }


        /**
         * @brief Set the state backlink.
         */
        void setBacklink(int backlink) {
            backlink_ = backlink;
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
        std::vector<StateTrans> shifts_;
        std::vector<StateTrans> gotos_;

        std::vector<int> backlinks_;

        Symbol match_;
        int backlink_ = {};

        int id_ = {};
    };

}
