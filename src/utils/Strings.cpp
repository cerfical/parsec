#include "utils/Strings.hpp"
#include "utils/char_utils.hpp"

namespace parsec::utils {
	std::string Strings::capitalize(std::string_view str) {
		std::string res;
		if(!str.empty()) {
			res += char_utils::toUpper(str.front());
			res += toLower(str.substr(1));
		}
		return res;
	}


	std::string Strings::toLower(std::string_view str) {
		std::string lower;
		for(const auto& ch : str) {
			lower += char_utils::toLower(ch);
		}
		return lower;
	}


	std::string Strings::toUpper(std::string_view str) {
		std::string upper;
		for(const auto& ch : str) {
			upper += char_utils::toUpper(ch);
		}
		return upper;
	}
}