#pragma once

#include <cctype>
#include <string>

/**
 * @brief Utilities for working with characters.
 */
namespace parsec::text::chars {

    /** @{ */
    /** @name Character conversion */
    /**
     * @brief Convert a character to an integer safely.
     */
    inline int toInt(char ch) noexcept {
        return static_cast<unsigned char>(ch);
    }


    /**
     * @brief Convert a character to lowercase.
     */
    inline char toLower(char ch) noexcept {
        return static_cast<char>(std::tolower(toInt(ch)));
    }


    /**
     * @brief Convert a character to uppercase.
     */
    inline char toUpper(char ch) noexcept {
        return static_cast<char>(std::toupper(toInt(ch)));
    }


    /**
     * @brief Find the value of a digit character.
     */
    inline int evalDigit(char ch) noexcept {
        return ch - '0';
    }


    /**
     * @brief Find the value of a hexadecimal digit character.
     */
    inline int evalHexDigit(char ch) noexcept {
        if(const auto lch = toLower(ch); lch >= 'a') {
            return lch - 'a' + 10;
        }
        return evalDigit(ch);
    }


    /**
     * @brief Convert a character to its text form, escaping non-printable characters.
     */
    std::string escape(char ch);
    /** @} */


    /** @{ */
    /** @name Character classification */
    /**
     * @brief Check if a character is lowercase.
     */
    inline bool isLower(char ch) noexcept {
        return std::islower(toInt(ch)) != 0;
    }


    /**
     * @brief Check if a character is uppercase.
     */
    inline bool isUpper(char ch) noexcept {
        return std::isupper(toInt(ch)) != 0;
    }


    /**
     * @brief Check if a character is an alphabetic character.
     */
    inline bool isAlpha(char ch) noexcept {
        return std::isalpha(toInt(ch)) != 0;
    }


    /**
     * @brief Check if a character is a digit.
     */
    inline bool isDigit(char ch) noexcept {
        return std::isdigit(toInt(ch)) != 0;
    }


    /**
     * @brief Check if a character is a hexadecimal digit.
     */
    inline bool isHexDigit(char ch) noexcept {
        return std::isxdigit(toInt(ch)) != 0;
    }


    /**
     * @brief Check if a character is an alphanumeric character.
     */
    inline bool isAlnum(char ch) noexcept {
        return std::isalnum(toInt(ch)) != 0;
    }


    /**
     * @brief Check if a character is a whitespace character.
     */
    inline bool isSpace(char ch) noexcept {
        return std::isspace(toInt(ch)) != 0;
    }


    /**
     * @brief Check if a character is a printable character.
     */
    inline bool isPrint(char ch) noexcept {
        return std::isprint(toInt(ch)) != 0;
    }
    /** @} */

}
