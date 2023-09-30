#include "Utils.hpp"

#include <iomanip>
#include <sstream>

namespace parsec {
	std::string CharUtils::EscapeChar(char ch) {
		// for control characters, return the appropriate escape sequence
		switch(ch) {
			case '\'': return "\\\'";
			case '\"': return "\\\"";
			case '\?': return "\\?";
			case '\\': return "\\\\";
			case '\a': return "\\a";
			case '\b': return "\\b";
			case '\f': return "\\f";
			case '\n': return "\\n";
			case '\r': return "\\r";
			case '\t': return "\\t";
			case '\v': return "\\v";
			case '\0': return "\\0";
		}
		
		// printable characters represent themselves
		if(IsPrint(ch)) {
			return std::string(1, ch);
		}

		// non-printable characters are transformed to their numeric codes
		return (std::ostringstream()
			<< "\\x" << std::setfill('0') << std::setw(2) << std::hex << CharToInt(ch)
		).str();
	}
}