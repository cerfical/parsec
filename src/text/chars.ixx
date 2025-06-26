module;

#include <cctype>
#include <string>

export module parsec.text:chars;

namespace parsec::text {

    /** @{ */
    /** @name Character conversion */
    /**
     * @brief Convert a character to an integer safely.
     */
    export int toInt(char ch) noexcept {
        return static_cast<unsigned char>(ch);
    }


    /**
     * @brief Convert a character to lowercase.
     */
    export char toLower(char ch) noexcept {
        return static_cast<char>(std::tolower(toInt(ch)));
    }


    /**
     * @brief Convert a character to uppercase.
     */
    export char toUpper(char ch) noexcept {
        return static_cast<char>(std::toupper(toInt(ch)));
    }


    /**
     * @brief Find the value of a digit character.
     */
    export int evalDigit(char ch) noexcept {
        return ch - '0';
    }


    /**
     * @brief Find the value of a hexadecimal digit character.
     */
    export int evalHexDigit(char ch) noexcept {
        if(const auto lch = toLower(ch); lch >= 'a') {
            return lch - 'a' + 10;
        }
        return evalDigit(ch);
    }


    /**
     * @brief Convert a character to its text form, escaping non-printable characters.
     */
    export std::string escape(char ch);
    /** @} */


    /** @{ */
    /** @name Character classification */
    /**
     * @brief Check if a character is lowercase.
     */
    export bool isLower(char ch) noexcept {
        return std::islower(toInt(ch)) != 0;
    }


    /**
     * @brief Check if a character is uppercase.
     */
    export bool isUpper(char ch) noexcept {
        return std::isupper(toInt(ch)) != 0;
    }


    /**
     * @brief Check if a character is an alphabetic character.
     */
    export bool isAlpha(char ch) noexcept {
        return std::isalpha(toInt(ch)) != 0;
    }


    /**
     * @brief Check if a character is a digit.
     */
    export bool isDigit(char ch) noexcept {
        return std::isdigit(toInt(ch)) != 0;
    }


    /**
     * @brief Check if a character is a hexadecimal digit.
     */
    export bool isHexDigit(char ch) noexcept {
        return std::isxdigit(toInt(ch)) != 0;
    }


    /**
     * @brief Check if a character is an alphanumeric character.
     */
    export bool isAlnum(char ch) noexcept {
        return std::isalnum(toInt(ch)) != 0;
    }


    /**
     * @brief Check if a character is a whitespace character.
     */
    export bool isSpace(char ch) noexcept {
        return std::isspace(toInt(ch)) != 0;
    }


    /**
     * @brief Check if a character is a printable character.
     */
    export bool isPrint(char ch) noexcept {
        return std::isprint(toInt(ch)) != 0;
    }
    /** @} */

}
