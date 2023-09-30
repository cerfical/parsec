#include "utils/SourceLoc.hpp"

namespace parsec {
	std::ostream& operator<<(std::ostream& out, const SourceLoc& loc) {
		out << loc.line() << ':' << loc.startCol() << '-' << loc.endCol();
		return out;
	}
}