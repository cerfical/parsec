#include "utils/char_utils.hpp"
#include <format>

namespace parsec::char_utils {
	std::string escape(char ch) {
		if(isPrint(ch)) {
			switch(ch) {
				case '\\': return "\\\\";
				case '\'': return "\\\'";
				case '\"': return "\\\"";
				default: return std::string(1, ch);
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
			default: {
				return std::format("\\x{:02x}", toInt(ch));
			}
		}
	}
}