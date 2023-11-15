#include "fg/Symbol.hpp"

namespace parsec::fg {
	std::ostream& operator<<(std::ostream& out, const Symbol& sym) {
		out << "{ " << sym.type() << ": " << sym.name();
		if(sym.rule()) {
			out << ": " << *sym.rule();
		}
		out << " }";
		return out;
	}
}