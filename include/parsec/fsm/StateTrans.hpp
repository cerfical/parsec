#pragma once

#include "../core/Symbol.hpp"

namespace parsec::fsm {

    /**
     * @brief Describes a state transition of an automaton.
     */
    struct StateTrans {

        /**
         * @brief Target state of the transition.
         */
        int target = {};


        /**
         * @brief Input symbol under which the transition is applied.
         */
        Symbol label;
    };

}
