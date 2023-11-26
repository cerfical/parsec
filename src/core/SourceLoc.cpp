#include "core/SourceLoc.hpp"

namespace parsec {
	std::ostream& operator<<(std::ostream& out, const SourceLoc& loc) {
		out << loc.lineNo() + 1 << ':' << loc.startCol() + 1;
		if(loc.colCount()) {
			out << '-' << (loc.endCol() - 1) + 1;
		}
		return out;
	}
}