#pragma once

#include <string>
#include <string_view>

/**
 * @brief Utilities for working with character strings.
 */
namespace parsec::string_util {

    /**
     * @brief Make up a valid PascalCase C++ identifier from a string.
     */
    std::string toPascalCase(std::string_view str);


    /**
     * @brief Make a first letter of a word uppercased.
     */
    std::string capitalize(std::string_view str);


    /**
     * @brief Convert all letters in a word to lowercase.
     */
    std::string toLower(std::string_view str);


    /**
     * @brief Convert all letters in a word to uppercase.
     */
    std::string toUpper(std::string_view str);


    /**
     * @brief Present a string in a human-readable form.
     */
    std::string escape(std::string_view str);

}
