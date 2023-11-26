#include "utils/Strings.hpp"
#include "utils/Chars.hpp"

namespace parsec::utils {
	std::string Strings::capitalize(std::string_view s) {
		auto capitalized = toLower(s);
		if(!capitalized.empty()) {
			capitalized.front() = Chars::toUpper(capitalized.front());
		}
		return capitalized;
	}


	std::string Strings::toLower(std::string_view s) {
		std::string lower;
		for(const auto ch : s) {
			lower += Chars::toLower(ch);
		}
		return lower;
	}

	std::string Strings::toUpper(std::string_view s) {
		std::string upper;
		for(const auto ch : s) {
			upper += Chars::toUpper(ch);
		}
		return upper;
	}
}