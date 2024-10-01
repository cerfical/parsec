#pragma once

#include "../core/Symbol.hpp"

#include <ostream>

namespace parsec::fsm {

    struct StateTrans {
        int target = {};
        Symbol label;
    };

    std::ostream& operator<<(std::ostream& out, const StateTrans& trans);

}
