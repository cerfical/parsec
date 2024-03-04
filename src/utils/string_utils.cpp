#include "utils/string_utils.hpp"
#include "utils/char_utils.hpp"

namespace parsec::string_utils {
	std::string toPascalCase(std::string_view str) {
		// remove leading non-alphabetic characters
		std::size_t wordStart = 0;
		while(wordStart < str.size() && !char_utils::isAlpha(str[wordStart])) {
			wordStart++;
		}

		std::string res;
		while(wordStart < str.size()) {
			bool anyUpper = false, anyLower = false;
			std::size_t wordEnd = wordStart;

			// parse a word made up of a sequence of alphanumeric characters
			while(wordEnd < str.size() && char_utils::isAlnum(str[wordEnd])) {
				if(char_utils::isLower(str[wordEnd])) {
					anyUpper = true;
				} else if(char_utils::isUpper(str[wordEnd])) {
					anyLower = true;
				}
				wordEnd++;
			}

			// convert the word to PascalCase, preserving the existing formatting
			const auto word = str.substr(wordStart, wordEnd - wordStart);
			if(anyLower == anyUpper) {
				res += char_utils::toUpper(word.front());
				res += word.substr(1);
			} else {
				res += string_utils::capitalize(word);
			}

			// remove delimiting non-alphabetic characters
			while(wordEnd < str.size() && !char_utils::isAlnum(str[wordEnd])) {
				wordEnd++;
			}
			wordStart = wordEnd;
		}
		return res;
	}


	std::string capitalize(std::string_view str) {
		std::string res;
		if(!str.empty()) {
			res += char_utils::toUpper(str.front());
			res += toLower(str.substr(1));
		}
		return res;
	}


	std::string toLower(std::string_view str) {
		std::string lower;
		for(const auto& ch : str) {
			lower += char_utils::toLower(ch);
		}
		return lower;
	}


	std::string toUpper(std::string_view str) {
		std::string upper;
		for(const auto& ch : str) {
			upper += char_utils::toUpper(ch);
		}
		return upper;
	}
}