#ifndef PARSEC_ERRORS_UNEXPECTED_CHAR_ERROR_HEADER
#define PARSEC_ERRORS_UNEXPECTED_CHAR_ERROR_HEADER

#include "ParseError.hpp"

namespace parsec {

	class UnexpectedCharError : public ParseError {
	public:

		explicit UnexpectedCharError(const SourceLoc& loc, char ch)
			: ParseError(ErrorCodes::UnexpectedChar, loc), m_ch(ch) {}

		const char* what() const override;

	private:
		mutable std::string m_msg;
		char m_ch = {};
	};

}

#endif