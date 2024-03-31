#ifndef PARSEC_ERRORS_PARSE_ERROR_HEADER
#define PARSEC_ERRORS_PARSE_ERROR_HEADER

#include "../core/SourceLoc.hpp"
#include "Error.hpp"

namespace parsec {

	class ParseError : public Error {
	public:

		ParseError(ErrorCodes code, const SourceLoc& loc)
			: Error(code), m_loc(loc) {}

		const SourceLoc& loc() const {
			return m_loc;
		}

	private:
		SourceLoc m_loc;
	};

}

#endif