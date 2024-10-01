#pragma once

#include "../core/SymbolGrammar.hpp"

#include "ElrState.hpp"

#include <vector>

namespace parsec::fsm {

    class ElrAutomaton {
    public:

        ElrAutomaton(const ElrAutomaton& other) = default;
        ElrAutomaton& operator=(const ElrAutomaton& other) = default;

        ElrAutomaton(ElrAutomaton&& other) noexcept = default;
        ElrAutomaton& operator=(ElrAutomaton&& other) noexcept = default;

        ~ElrAutomaton() noexcept = default;


        ElrAutomaton() noexcept = default;

        explicit ElrAutomaton(const SymbolGrammar& grammar);


        const std::vector<ElrState>& states() const noexcept {
            return states_;
        }


    private:
        class StateBuilder;
        std::vector<ElrState> states_;
    };

}
