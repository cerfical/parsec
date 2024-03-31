#ifndef PARSEC_ERRORS_UNEXPECTED_EOF_ERROR_HEADER
#define PARSEC_ERRORS_UNEXPECTED_EOF_ERROR_HEADER

#include "ParseError.hpp"

namespace parsec {

	class UnexpectedEofError : public ParseError {
	public:

		explicit UnexpectedEofError(const SourceLoc& loc)
			: ParseError(ErrorCodes::UnexpectedEof, loc) {}

	};

}

#endif