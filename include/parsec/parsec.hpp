#pragma once

#include "core/CodeGen.hpp"
#include "core/CompileError.hpp"
#include "core/Compiler.hpp"
#include "core/NameConflictError.hpp"
#include "core/SymbolGrammar.hpp"

/**
 * @brief Root namespace for the parsec library.
 */
namespace parsec {

    /**
     * @brief Provides functionality to work with regular expressions.
     */
    namespace regex {}

    /**
     * @brief Defines syntax and parsing capabilities for grammar specifications.
     */
    namespace pars {}

    /**
     * @brief Provides facilities for constructing finite state machines of different kinds.
     */
    namespace fsm {}

}
