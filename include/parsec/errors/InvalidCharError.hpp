#ifndef PARSEC_ERRORS_INVALID_CHAR_ERROR_HEADER
#define PARSEC_ERRORS_INVALID_CHAR_ERROR_HEADER

#include "ParseError.hpp"

namespace parsec {

	class InvalidCharError : public ParseError {
	public:

		explicit InvalidCharError(const SourceLoc& loc, char ch)
			: ParseError(ErrorCodes::InvalidChar, loc), m_ch(ch) {}

		const char* what() const override;

	private:
		mutable std::string m_msg;
		char m_ch = {};
	};

}

#endif