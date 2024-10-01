#pragma once

#include "StateTrans.hpp"

#include <vector>

namespace parsec::fsm {

    class ElrState {
    public:

        ElrState(const ElrState& other) = default;
        ElrState& operator=(const ElrState& other) = default;

        ElrState(ElrState&& other) noexcept = default;
        ElrState& operator=(ElrState&& other) noexcept = default;

        ~ElrState() noexcept = default;


        ElrState() noexcept = default;

        explicit ElrState(int id)
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


        const std::vector<int>& backlinks() const noexcept {
            return backlinks_;
        }

        void addBacklink(int backlink) {
            backlinks_.emplace_back(backlink);
        }


        const Symbol& match() const noexcept {
            return match_;
        }

        void setMatch(const Symbol& match) {
            match_ = match;
        }


        int backlink() const noexcept {
            return backlink_;
        }

        void setBacklink(int backlink) {
            backlink_ = backlink;
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
        std::vector<StateTrans> shifts_;
        std::vector<StateTrans> gotos_;

        std::vector<int> backlinks_;

        Symbol match_;
        int backlink_ = {};

        int id_ = {};
    };

}
