#ifndef PARSEC_CORE_PARSE_ERROR_HEADER
#define PARSEC_CORE_PARSE_ERROR_HEADER

#include "SourceLoc.hpp"
#include "Error.hpp"

namespace parsec {

	class ParseError : public Error {
	public:

		ParseError(const SourceLoc& loc, const std::string& msg)
			: Error(msg), m_loc(loc) {}

		const SourceLoc& loc() const {
			return m_loc;
		}

	private:
		SourceLoc m_loc;
	};

}

#endif