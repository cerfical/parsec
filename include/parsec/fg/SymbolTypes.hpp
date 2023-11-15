#ifndef PARSEC_FG_SYMBOL_TYPES_HEADER
#define PARSEC_FG_SYMBOL_TYPES_HEADER

#define PARSEC_FG_SYMBOL_TYPE_LIST \
	PARSEC_FG_SYMBOL_TYPE(None) \
	PARSEC_FG_SYMBOL_TYPE(Token) \
	PARSEC_FG_SYMBOL_TYPE(Rule)

#include <ostream>

namespace parsec::fg {
#define PARSEC_FG_SYMBOL_TYPE(sym) sym,
	
	enum class SymbolTypes {
		PARSEC_FG_SYMBOL_TYPE_LIST
	};

#undef PARSEC_FG_SYMBOL_TYPE


	std::ostream& operator<<(std::ostream& out, SymbolTypes sym);
}

#endif