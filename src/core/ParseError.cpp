#include "core/ParseError.hpp"
#include <format>

namespace parsec {
	ParseError ParseError::nameRedefine(const SourceLoc& loc) {
		return ParseError(loc, "name redefinition");
	}

	ParseError ParseError::reservedNameRedefine(const SourceLoc& loc) {
		return ParseError(loc, "trying to redefine a reserved name");
	}

	ParseError ParseError::emptyName(const SourceLoc& loc) {
		return ParseError(loc, "name must contain at least one letter");
	}

	ParseError ParseError::undefinedName(const SourceLoc& loc) {
		return ParseError(loc, "use of undefined name");
	}

	ParseError ParseError::patternConflict(const SourceLoc& loc, std::string_view conflictPattern) {
		return ParseError(loc, std::format("pattern is in conflict with pattern \"{}\"", conflictPattern));
	}

	ParseError ParseError::ruleConflict(const SourceLoc& loc, std::string_view conflictRule) {
		return ParseError(loc, std::format("rule is in conflict with rule \"{}\"", conflictRule));
	}
}