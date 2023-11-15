#include "fg/SymbolTypes.hpp"

namespace parsec::fg {
	std::ostream& operator<<(std::ostream& out, SymbolTypes sym) {
		switch(sym) {
#define PARSEC_FG_SYMBOL_TYPE(sym) \
			case SymbolTypes::sym: out << #sym; break;

			PARSEC_FG_SYMBOL_TYPE_LIST

#undef PARSEC_FG_SYMBOL_TYPE

			default: out << "UnknownSymbol"; break;
		}
		return out;
	}
}