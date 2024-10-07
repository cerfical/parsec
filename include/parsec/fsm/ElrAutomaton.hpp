#pragma once

#include "../core/SymbolGrammar.hpp"
#include "ElrState.hpp"

#include <vector>

namespace parsec::fsm {

    /**
     * @brief Describes a extended LR (ELR) automaton.
     */
    class ElrAutomaton {
    public:

        ElrAutomaton() = default;

        ElrAutomaton(const ElrAutomaton& other) = default;
        ElrAutomaton& operator=(const ElrAutomaton& other) = default;

        ElrAutomaton(ElrAutomaton&& other) noexcept = default;
        ElrAutomaton& operator=(ElrAutomaton&& other) noexcept = default;

        ~ElrAutomaton() = default;


        /** @{ */
        /**
         * @brief Construct an automaton that recognizes a language defined by a grammar.
         */
        explicit ElrAutomaton(const SymbolGrammar& grammar);


        /**
         * @brief List of states in the automaton.
         */
        const std::vector<ElrState>& states() const noexcept {
            return states_;
        }
        /** @} */


    private:
        class StateBuilder;
        std::vector<ElrState> states_;
    };

}
