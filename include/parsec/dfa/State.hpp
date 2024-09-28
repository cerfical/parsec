#pragma once

#include "../core/Symbol.hpp"

#include <ostream>
#include <vector>

namespace parsec::dfa {

    struct StateTrans {
        int target = {};
        Symbol label;
    };

    std::ostream& operator<<(std::ostream& out, const StateTrans& trans);


    class State {
    public:

        State() noexcept = default;

        explicit State(int id) noexcept
            : id_(id) {}


        const std::vector<StateTrans>& transitions() const noexcept {
            return transitions_;
        }


        void addTransition(int target, const Symbol& label) {
            transitions_.emplace_back(target, label);
        }


        bool isMatchState() const noexcept {
            return !matchedRule().isEmpty();
        }


        const Symbol& matchedRule() const noexcept {
            return matchedRule_;
        }


        void setMatchedRule(const Symbol& rule) noexcept {
            matchedRule_ = rule;
        }


        bool isStartState() const noexcept {
            return startState_;
        }


        void setStartState(bool startState) noexcept {
            startState_ = startState;
        }


        explicit operator bool() const noexcept {
            return !isEmpty();
        }


        bool isEmpty() const noexcept {
            return transitions_.empty() && !isStartState() && !isMatchState();
        }


        int id() const noexcept {
            return id_;
        }


    private:
        std::vector<StateTrans> transitions_;
        Symbol matchedRule_;

        int id_ = {};
        bool startState_ = false;
    };

}
