#include "LexPattern.hpp"
#include "RegExParser.hpp"

namespace parsec {
	LexPattern::LexPattern(const std::string& name, const std::string& pattern)
	 : pattern(pattern), name(name) {
		regex = std::make_unique<RegExConcat>(
			RegExParser().Parse(pattern),
			std::make_unique<RegExChar>('#')
		);
	}
}