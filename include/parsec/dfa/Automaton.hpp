#pragma once

#include "../core/SymbolGrammar.hpp"

#include "State.hpp"

#include <vector>

namespace parsec::dfa {

    class Automaton {
    public:

        Automaton(const Automaton& other) = delete;
        Automaton& operator=(const Automaton& other) = delete;

        Automaton(Automaton&& other) noexcept = default;
        Automaton& operator=(Automaton&& other) noexcept = default;

        ~Automaton() = default;


        Automaton() noexcept = default;

        explicit Automaton(const SymbolGrammar& grammar);

        explicit Automaton(const SymbolRule& pattern);


        const std::vector<State>& states() const noexcept {
            return states_;
        }


        const State& stateById(int state) const noexcept;


        const State& startState() const noexcept;


        explicit operator bool() const noexcept {
            return !isEmpty();
        }


        bool isEmpty() const noexcept {
            return states_.empty();
        }


    private:
        class StateBuilder;

        std::vector<State> states_;
    };

}
