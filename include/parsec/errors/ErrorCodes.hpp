#ifndef PARSEC_ERRORS_ERROR_CODES_HEADER
#define PARSEC_ERRORS_ERROR_CODES_HEADER

#include <system_error>

namespace parsec {

	enum class ErrorCodes {
		Success,
		UnexpectedEof,
		InvalidChar,
		UnexpectedToken,
		TokenMismatch,
		UnmatchedParenthesis
	};

}

namespace std {

	template <>
	struct is_error_code_enum<parsec::ErrorCodes> : public true_type {};

	template <>
	struct is_error_condition_enum<parsec::ErrorCodes> : public true_type {};

}

#endif