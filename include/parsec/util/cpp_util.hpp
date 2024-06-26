#pragma once

#include "../core/Symbol.hpp"

#include <span>
#include <string>

/**
 * @brief Auxiliary functions to facilitate C++ sources generation.
 */
namespace parsec::cpp_util {

    /**
     * @brief Make up the source code for a C++ enumeration.
     * @param enumName The name of the enumeration.
     * @param valueNames A list of names for the enumeration values.
     */
    std::string makeEnum(const Symbol& enumName, std::span<const Symbol> valueNames);

}
