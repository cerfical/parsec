#include "util/string_util.hpp"
#include "util/chars.hpp"

#include <cstddef>
#include <string>
#include <string_view>

namespace parsec::string_util {
    std::string toPascalCase(std::string_view str) {
        // remove leading non-alphabetic characters
        std::size_t wordStart = 0;
        while(wordStart < str.size() && !chars::isAlpha(str[wordStart])) {
            wordStart++;
        }

        std::string res;
        while(wordStart < str.size()) {
            bool anyUpper = false;
            bool anyLower = false;

            // parse a word made up of a sequence of alphanumeric characters
            std::size_t wordEnd = wordStart;
            while(wordEnd < str.size() && chars::isAlnum(str[wordEnd])) {
                if(chars::isLower(str[wordEnd])) {
                    anyUpper = true;
                } else if(chars::isUpper(str[wordEnd])) {
                    anyLower = true;
                }
                wordEnd++;
            }

            // convert the word to PascalCase, preserving the existing formatting
            const auto word = str.substr(wordStart, wordEnd - wordStart);
            if(anyLower == anyUpper) {
                res += chars::toUpper(word.front());
                res += word.substr(1);
            } else {
                res += string_util::capitalize(word);
            }

            // remove delimiting non-alphabetic characters
            while(wordEnd < str.size() && !chars::isAlnum(str[wordEnd])) {
                wordEnd++;
            }
            wordStart = wordEnd;
        }
        return res;
    }


    std::string toLower(std::string_view str) {
        std::string lower;
        for(char ch : str) {
            lower += chars::toLower(ch);
        }
        return lower;
    }


    std::string toUpper(std::string_view str) {
        std::string upper;
        for(char ch : str) {
            upper += chars::toUpper(ch);
        }
        return upper;
    }


    std::string escape(std::string_view str) {
        std::string escaped;
        for(char ch : str) {
            escaped += chars::escape(ch);
        }
        return escaped;
    }


    std::string capitalize(std::string_view str) {
        std::string capitalized;
        if(!str.empty()) {
            capitalized += chars::toUpper(str.front());
            capitalized += toLower(str.substr(1));
        }
        return capitalized;
    }
}
