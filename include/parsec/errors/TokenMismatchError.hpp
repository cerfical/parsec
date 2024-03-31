#ifndef PARSEC_ERRORS_TOKEN_MISMATCH_ERROR_HEADER
#define PARSEC_ERRORS_TOKEN_MISMATCH_ERROR_HEADER

#include "ParseError.hpp"

namespace parsec {

	class TokenMismatchError : public ParseError {
	public:

		explicit TokenMismatchError(const SourceLoc& loc, const std::string& expect, const std::string& got)
			: ParseError(ErrorCodes::TokenMismatch, loc), m_expect(expect), m_got(got) {}

		const char* what() const override;

	private:
		mutable std::string m_msg;
		std::string m_expect;
		std::string m_got;
	};

}

#endif