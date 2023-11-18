#include "utils/strings.hpp"
#include "utils/chars.hpp"

namespace parsec {
	std::string Strings::capitalize(const std::string& str) {
		auto capitalized = toLower(str);
		if(!capitalized.empty()) {
			capitalized.front() = parsec::toUpper(capitalized.front());
		}
		return capitalized;
	}


	std::string Strings::toLower(const std::string& str) {
		std::string lower;
		for(const auto ch : str) {
			lower += parsec::toLower(ch);
		}
		return lower;
	}

	std::string Strings::toUpper(const std::string& str) {
		std::string upper;
		for(const auto ch : str) {
			upper += parsec::toUpper(ch);
		}
		return upper;
	}
}