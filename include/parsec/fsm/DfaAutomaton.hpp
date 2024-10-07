#pragma once

#include "../core/SymbolGrammar.hpp"
#include "DfaState.hpp"

#include <vector>

namespace parsec::fsm {

    /**
     * @brief Describes a deterministic finite automaton (DFA).
     */
    class DfaAutomaton {
    public:

        DfaAutomaton() = default;

        DfaAutomaton(const DfaAutomaton& other) = default;
        DfaAutomaton& operator=(const DfaAutomaton& other) = default;

        DfaAutomaton(DfaAutomaton&& other) noexcept = default;
        DfaAutomaton& operator=(DfaAutomaton&& other) noexcept = default;

        ~DfaAutomaton() = default;


        /** @{ */
        /**
         * @brief Construct an automaton that recognizes a language defined by a grammar.
         */
        explicit DfaAutomaton(const SymbolGrammar& grammar);


        /**
         * @brief List of states in the automaton.
         */
        const std::vector<DfaState>& states() const noexcept {
            return states_;
        }
        /** @} */


    private:
        class StateBuilder;
        std::vector<DfaState> states_;
    };

}
