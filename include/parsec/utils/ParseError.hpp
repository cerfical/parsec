#ifndef PARSEC_UTILS_PARSE_ERROR_HEADER
#define PARSEC_UTILS_PARSE_ERROR_HEADER

#include "SourceLoc.hpp"

#include <stdexcept>
#include <gsl/gsl>

namespace parsec {
	/**
	 * @brief Indicates a syntax error in the source code.
	 */
	class ParseError : public std::runtime_error {
	public:
		/** @{ */
		/** @brief Construct a new error from a C-style string message and a location. */
		ParseError(gsl::czstring msg, const SourceLoc& loc)
			: runtime_error(msg), m_loc(loc)
		{ }

		/** @brief Construct a new error from a string message and a location. */
		ParseError(const std::string& msg, const SourceLoc& loc)
			: runtime_error(msg), m_loc(loc)
		{ }
		/** @} */

		/** @{ */
		/** @brief Location in the source code where the error occurred. */
		const SourceLoc& location() const noexcept {
			return m_loc;
		}
		/** @} */

	private:
		SourceLoc m_loc;
	};
}

#endif