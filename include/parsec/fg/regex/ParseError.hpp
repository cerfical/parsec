#ifndef PARSEC_FG_REGEX_PARSE_ERROR_HEADER
#define PARSEC_FG_REGEX_PARSE_ERROR_HEADER

#include "../../core/Error.hpp"
#include "../../core/IndexRange.hpp"

namespace parsec::fg::regex {

	class ParseError : public Error {
	public:

		ParseError(const std::string& msg, const IndexRange& loc)
			: Error(msg), m_loc(loc) {}

		const IndexRange& loc() const {
			return m_loc;
		}

	private:
		IndexRange m_loc;
	};

}

#endif