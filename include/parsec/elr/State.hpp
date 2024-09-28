#pragma once

#include "../core/Symbol.hpp"

#include <vector>

namespace parsec::elr {

    struct StateTrans {
        int target = {};
        Symbol label;
    };


    struct ReduceAction {
        Symbol reduceRule;
        int backLink = {};
    };


    class State {
    public:

        State() noexcept = default;

        explicit State(int id) noexcept
            : id_(id) {}


        const std::vector<StateTrans>& shifts() const noexcept {
            return shifts_;
        }


        void addShift(int target, const Symbol& label) {
            shifts_.emplace_back(target, label);
        }


        const std::vector<StateTrans>& gotos() const noexcept {
            return gotos_;
        }


        void addGoto(int target, const Symbol& label) {
            gotos_.emplace_back(target, label);
        }


        const std::vector<int>& backLinks() const noexcept {
            return backLinks_;
        }


        void addBackLink(int backLink) {
            backLinks_.emplace_back(backLink);
        }


        bool isReduceState() const noexcept {
            return !reduction().reduceRule.isEmpty();
        }


        const ReduceAction& reduction() const noexcept {
            return reduction_;
        }


        void setReduction(const Symbol& reduceRule, int backLink) noexcept {
            reduction_ = ReduceAction(reduceRule, backLink);
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
            return shifts_.empty() && gotos_.empty() && !isStartState() && !isReduceState();
        }


        int id() const noexcept {
            return id_;
        }


    private:
        std::vector<StateTrans> shifts_;
        std::vector<StateTrans> gotos_;

        std::vector<int> backLinks_;
        ReduceAction reduction_;

        int id_ = {};
        bool startState_ = false;
    };

}
