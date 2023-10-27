#ifndef PARSEC_UTILS_PARSE_ERROR_HEADER
#define PARSEC_UTILS_PARSE_ERROR_HEADER

#include "SourceLoc.hpp"
#include <stdexcept>

namespace parsec {
	/**
	 * @brief Indicates a syntax error in the source code.
	 */
	class ParseError : public std::runtime_error {
	public:
		/** @{ */
		/** @brief Construct an error with a message and some location information. */
		ParseError(const std::string& msg, const SourceLoc& loc)
			: runtime_error(msg), m_loc(loc)
		{ }
		/** @} */


		/** @{ */
		/** @brief Location in the source code where the error occurred. */
		const SourceLoc& loc() const noexcept {
			return m_loc;
		}
		/** @} */


	private:
		/** @{ */
		SourceLoc m_loc;
		/** @} */
	};
}

#endif