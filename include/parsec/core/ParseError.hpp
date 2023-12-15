#ifndef PARSEC_PARSE_ERROR_HEADER
#define PARSEC_PARSE_ERROR_HEADER

#include "SourceLoc.hpp"
#include "Error.hpp"

namespace parsec {

	/**
	 * @brief Indicates a syntax error in the source code.
	 */
	class ParseError : public Error {
	public:
		
		ParseError(const std::string& msg, const SourceLoc& loc)
			: Error(msg), m_loc(loc)
		{ }


		/** @brief Location in the source code where the error occurred. */
		const SourceLoc& loc() const noexcept {
			return m_loc;
		}


	private:
		SourceLoc m_loc;
	};

}

#endif