#ifndef PARSEC_CORE_ERROR_HEADER
#define PARSEC_CORE_ERROR_HEADER

#include <stdexcept>

namespace parsec {

	class Error : public std::runtime_error {
	public:

		explicit Error(const std::string& msg)
			: runtime_error(msg) {}

	};

}

#endif