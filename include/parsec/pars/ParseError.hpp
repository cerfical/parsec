#ifndef PARSEC_PARS_PARSE_ERROR_HEADER
#define PARSEC_PARS_PARSE_ERROR_HEADER

#include "../core/SourceLoc.hpp"
#include "../core/Error.hpp"

namespace parsec::pars {

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