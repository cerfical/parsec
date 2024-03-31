#ifndef PARSEC_ERRORS_UNEXPECTED_TOKEN_ERROR_HEADER
#define PARSEC_ERRORS_UNEXPECTED_TOKEN_ERROR_HEADER

#include "ParseError.hpp"

namespace parsec {

	class UnexpectedTokenError : public ParseError {
	public:

		explicit UnexpectedTokenError(const SourceLoc& loc, const std::string& tok)
			: ParseError(ErrorCodes::UnexpectedToken, loc), m_tok(tok) {}

		const char* what() const override;

	private:
		mutable std::string m_msg;
		std::string m_tok;
	};

}

#endif