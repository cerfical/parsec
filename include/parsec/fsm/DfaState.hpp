#pragma once

#include "StateTrans.hpp"

#include <vector>

namespace parsec::fsm {

    class DfaState {
    public:

        DfaState(const DfaState& other) = default;
        DfaState& operator=(const DfaState& other) = default;

        DfaState(DfaState&& other) noexcept = default;
        DfaState& operator=(DfaState&& other) noexcept = default;

        ~DfaState() noexcept = default;


        DfaState() noexcept = default;

        explicit DfaState(int id)
            : id_(id) {}


        const std::vector<StateTrans>& transitions() const noexcept {
            return transitions_;
        }

        void addTransition(int target, const Symbol& label) {
            transitions_.emplace_back(target, label);
        }


        const Symbol& match() const noexcept {
            return match_;
        }

        void setMatch(const Symbol& match) {
            match_ = match;
        }


        bool isStartState() const noexcept {
            return id() == 0;
        }

        bool isMatchState() const noexcept {
            return !match().isEmpty();
        }


        int id() const noexcept {
            return id_;
        }


    private:
        std::vector<StateTrans> transitions_;
        Symbol match_;

        int id_ = {};
    };

}
