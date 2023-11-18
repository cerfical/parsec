#include "utils/Strings.hpp"
#include "utils/Chars.hpp"

namespace parsec {
	std::string Strings::capitalize(const std::string& str) {
		auto capitalized = toLower(str);
		if(!capitalized.empty()) {
			capitalized.front() = Chars::toUpper(capitalized.front());
		}
		return capitalized;
	}


	std::string Strings::toLower(const std::string& str) {
		std::string lower;
		for(const auto ch : str) {
			lower += Chars::toLower(ch);
		}
		return lower;
	}

	std::string Strings::toUpper(const std::string& str) {
		std::string upper;
		for(const auto ch : str) {
			upper += Chars::toUpper(ch);
		}
		return upper;
	}
}