#ifndef PARSEC_ERR_HEADER
#define PARSEC_ERR_HEADER

#include "core/SourceLoc.hpp"
#include <string_view>

namespace parsec::err {

	[[noreturn]]
	void unexpectedEof(const SourceLoc& eofLoc);


	[[noreturn]]
	void invalidChar(const SourceLoc& charLoc, char ch);


	[[noreturn]]
	void emptyHexCharSeq(const SourceLoc& hexSeqLoc);


	[[noreturn]]
	void outOfOrderCharRange(const SourceLoc& charRangeLoc);


	[[noreturn]]
	void misplacedChar(const SourceLoc& charLoc, char ch);


	[[noreturn]]
	void misplacedToken(const SourceLoc& tokLoc, std::string_view tokText);


	[[noreturn]]
	void unmatchedToken(const SourceLoc& loc, std::string_view expect, std::string_view got);

}

#endif