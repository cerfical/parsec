#include "utils/chars.hpp"

#include <sstream>
#include <iomanip>

namespace parsec {
	std::string escapeChar(char ch) {
		// printable characters are printed (???)
		if(isPrint(ch)) {
			switch(ch) {
				case '\\': return "\\\\";
				case '\'': return "\\\'";
				case '\"': return "\\\"";
				default: return std::string(1, ch);
			}
		}

		// non-printable characters are converted to numerical values
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
				return (std::ostringstream()
					<< "\\x" << std::setw(2) << std::setfill('0') << std::hex << charToInt(ch)
				).str();
			}
		}
	}
}