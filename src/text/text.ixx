module;

#include <string>
#include <string_view>

export module parsec.text;

export import :chars;

/**
 * @brief Utilities for working with character strings.
 */
namespace parsec::text {

    /** @{ */
    /** @name String conversion */
    /**
     * @brief Convert a string to a valid PascalCase identifier.
     */
    export std::string toPascalCase(std::string_view str);


    /**
     * @brief Convert all letters in a string to lowercase.
     */
    export std::string toLower(std::string_view str) {
        std::string lower;
        for(char ch : str) {
            lower += toLower(ch);
        }
        return lower;
    }


    /**
     * @brief Convert all letters in a string to uppercase.
     */
    export std::string toUpper(std::string_view str) {
        std::string upper;
        for(char ch : str) {
            upper += toUpper(ch);
        }
        return upper;
    }


    /**
     * @brief Convert a string to its text form, escaping non-printable characters.
     */
    export std::string escape(std::string_view str) {
        std::string escaped;
        for(char ch : str) {
            escaped += escape(ch);
        }
        return escaped;
    }


    /**
     * @brief Convert a string into a new string with first character in uppercase.
     */
    export std::string capitalize(std::string_view str) {
        std::string capitalized;
        if(!str.empty()) {
            capitalized += toUpper(str.front());
            capitalized += toLower(str.substr(1));
        }
        return capitalized;
    }
    /** @} */


    std::string toPascalCase(std::string_view str) {
        // remove leading non-alphabetic characters
        std::size_t wordStart = 0;
        while(wordStart < str.size() && !isAlpha(str[wordStart])) {
            wordStart++;
        }

        std::string res;
        while(wordStart < str.size()) {
            bool anyUpper = false;
            bool anyLower = false;

            // parse a word made up of a sequence of alphanumeric characters
            std::size_t wordEnd = wordStart;
            while(wordEnd < str.size() && isAlnum(str[wordEnd])) {
                if(isLower(str[wordEnd])) {
                    anyUpper = true;
                } else if(isUpper(str[wordEnd])) {
                    anyLower = true;
                }
                wordEnd++;
            }

            // convert the word to PascalCase, preserving the existing formatting
            const auto word = str.substr(wordStart, wordEnd - wordStart);
            if(anyLower == anyUpper) {
                res += toUpper(word.front());
                res += word.substr(1);
            } else {
                res += capitalize(word);
            }

            // remove delimiting non-alphabetic characters
            while(wordEnd < str.size() && !isAlnum(str[wordEnd])) {
                wordEnd++;
            }
            wordStart = wordEnd;
        }
        return res;
    }

}
