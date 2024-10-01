#pragma once

#include "../core/SymbolGrammar.hpp"

#include "DfaState.hpp"

#include <vector>

namespace parsec::fsm {

    class DfaAutomaton {
    public:

        DfaAutomaton(const DfaAutomaton& other) = default;
        DfaAutomaton& operator=(const DfaAutomaton& other) = default;

        DfaAutomaton(DfaAutomaton&& other) noexcept = default;
        DfaAutomaton& operator=(DfaAutomaton&& other) noexcept = default;

        ~DfaAutomaton() noexcept = default;


        DfaAutomaton() noexcept = default;

        explicit DfaAutomaton(const SymbolGrammar& grammar);


        const std::vector<DfaState>& states() const noexcept {
            return states_;
        }


    private:
        class StateBuilder;
        std::vector<DfaState> states_;
    };

}
