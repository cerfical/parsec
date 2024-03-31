#ifndef PARSEC_ERRORS_ERROR_HEADER
#define PARSEC_ERRORS_ERROR_HEADER

#include "ErrorCodes.hpp"
#include <system_error>

namespace parsec {

	class Error : public std::system_error {
	public:

		explicit Error(ErrorCodes code);

	};

}

#endif