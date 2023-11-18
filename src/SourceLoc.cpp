#include "SourceLoc.hpp"

namespace parsec {
	std::ostream& operator<<(std::ostream& out, const SourceLoc& loc) {
		out << loc.lineNo() + 1 << ':' << loc.startCol() + 1 << '-' << loc.endCol() + 1;
		return out;
	}
}