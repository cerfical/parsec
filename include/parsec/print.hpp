#pragma once

#include "core/SymbolGrammar.hpp"

#include "dfa/Automaton.hpp"
#include "dfa/State.hpp"

#include "elr/Automaton.hpp"
#include "elr/State.hpp"

#include <iostream>
#include <string_view>

namespace parsec {
    void print(const SymbolGrammar& grammar, std::ostream& out = std::cout, std::string_view indent = "");
}

namespace parsec::elr {
    void print(const State& state, std::ostream& out = std::cout, std::string_view indent = "");

    void print(const Automaton& elr, std::ostream& out = std::cout, std::string_view indent = "");
}

namespace parsec::dfa {
    void print(const State& state, std::ostream& out = std::cout, std::string_view indent = "");

    void print(const Automaton& dfa, std::ostream& out = std::cout, std::string_view indent = "");
}
