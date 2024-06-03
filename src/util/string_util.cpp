#include "util/string_util.hpp"
#include "util/char_util.hpp"

namespace parsec::string_util {
	std::string toPascalCase(std::string_view str) {
		// remove leading non-alphabetic characters
		std::size_t wordStart = 0;
		while(wordStart < str.size() && !char_util::isAlpha(str[wordStart])) {
			wordStart++;
		}

		std::string res;
		while(wordStart < str.size()) {
			bool anyUpper = false, anyLower = false;
			std::size_t wordEnd = wordStart;

			// parse a word made up of a sequence of alphanumeric characters
			while(wordEnd < str.size() && char_util::isAlnum(str[wordEnd])) {
				if(char_util::isLower(str[wordEnd])) {
					anyUpper = true;
				} else if(char_util::isUpper(str[wordEnd])) {
					anyLower = true;
				}
				wordEnd++;
			}

			// convert the word to PascalCase, preserving the existing formatting
			const auto word = str.substr(wordStart, wordEnd - wordStart);
			if(anyLower == anyUpper) {
				res += char_util::toUpper(word.front());
				res += word.substr(1);
			} else {
				res += string_util::capitalize(word);
			}

			// remove delimiting non-alphabetic characters
			while(wordEnd < str.size() && !char_util::isAlnum(str[wordEnd])) {
				wordEnd++;
			}
			wordStart = wordEnd;
		}
		return res;
	}


	std::string capitalize(std::string_view str) {
		std::string res;
		if(!str.empty()) {
			res += char_util::toUpper(str.front());
			res += toLower(str.substr(1));
		}
		return res;
	}


	std::string toLower(std::string_view str) {
		std::string lower;
		for(const auto& ch : str) {
			lower += char_util::toLower(ch);
		}
		return lower;
	}


	std::string toUpper(std::string_view str) {
		std::string upper;
		for(const auto& ch : str) {
			upper += char_util::toUpper(ch);
		}
		return upper;
	}


	std::string escape(std::string_view str) {
		std::string escapedStr;
		for(const auto& ch : str) {
			escapedStr += char_util::escape(ch);
		}
		return escapedStr;
	}
}