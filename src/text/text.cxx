module;

#include <format>
#include <string>
#include <string_view>

module parsec.text;

namespace parsec::text {

    std::string escape(char ch) {
        if(isPrint(ch)) {
            switch(ch) {
                case '\\': return "\\\\";
                case '\'': return "\\\'";
                case '\"': return "\\\"";
                default:   return std::string(1, ch);
            }
        }

        switch(ch) {
            case '\0': return "\\0";
            case '\a': return "\\a";
            case '\b': return "\\b";
            case '\f': return "\\f";
            case '\n': return "\\n";
            case '\r': return "\\r";
            case '\t': return "\\t";
            case '\v': return "\\v";
            default:   return std::format("\\x{:02x}", toInt(ch));
        }
    }

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
