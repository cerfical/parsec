#pragma once

#include "core/SymbolGrammar.hpp"

#include "fsm/DfaAutomaton.hpp"
#include "fsm/DfaState.hpp"
#include "fsm/ElrAutomaton.hpp"
#include "fsm/ElrState.hpp"

#include <iostream>
#include <string_view>

namespace parsec {
    void print(const SymbolGrammar& grammar, std::ostream& out = std::cout, std::string_view indent = "");
}

namespace parsec::fsm {
    void print(const ElrState& state, std::ostream& out = std::cout, std::string_view indent = "");

    void print(const ElrAutomaton& elr, std::ostream& out = std::cout, std::string_view indent = "");


    void print(const DfaState& state, std::ostream& out = std::cout, std::string_view indent = "");

    void print(const DfaAutomaton& dfa, std::ostream& out = std::cout, std::string_view indent = "");
}
