#pragma once

#include <string>
#include <string_view>

/**
 * @brief Utilities for working with character strings.
 */
namespace parsec::strings {

    /** @{ */
    /** @name String conversion */
    /**
     * @brief Convert a string to a valid PascalCase identifier.
     */
    std::string toPascalCase(std::string_view str);


    /**
     * @brief Convert all letters in a string to lowercase.
     */
    std::string toLower(std::string_view str);


    /**
     * @brief Convert all letters in a string to uppercase.
     */
    std::string toUpper(std::string_view str);


    /**
     * @brief Convert a string to its text form, escaping non-printable characters.
     */
    std::string escape(std::string_view str);


    /**
     * @brief Convert a string into a new string with first character in uppercase.
     */
    std::string capitalize(std::string_view str);
    /** @} */

}
