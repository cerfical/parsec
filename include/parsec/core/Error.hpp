#ifndef PARSEC_CORE_ERROR_HEADER
#define PARSEC_CORE_ERROR_HEADER

#include <stdexcept>

namespace parsec {

	/**
	 * @brief Base for all library error exceptions.
	 */
	class Error : public std::runtime_error {
	public:

		/** @brief Create a new exception object with an arbitrary error message. */
		explicit Error(const std::string& msg)
			: runtime_error(msg)
		{ }

	};

}

#endif