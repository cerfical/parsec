#ifndef PARSEC_ERROR_HEADER
#define PARSEC_ERROR_HEADER

#include "SourceLoc.hpp"
#include <stdexcept>

namespace parsec {
	/**
	 * @brief Indicates a syntax error in the source code.
	 */
	class Error : public std::runtime_error {
	public:
		Error(const std::string& msg, const SourceLoc& loc)
			: runtime_error(msg), m_loc(loc)
		{ }

		/** @{ */
		const SourceLoc& loc() const noexcept {
			return m_loc;
		}
		/** @} */

	private:
		SourceLoc m_loc;
	};
}

#endif