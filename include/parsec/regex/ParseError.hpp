#ifndef PARSEC_REGEX_PARSE_ERROR_HEADER
#define PARSEC_REGEX_PARSE_ERROR_HEADER

#include "../core/Error.hpp"
#include "../core/IndexRange.hpp"

#include <gsl/util>

namespace parsec::regex {

	/**
	 * @brief Thrown if a regular expression is ill-formed and cannot be properly parsed.
	*/
	class ParseError : public Error {
	public:

		ParseError(const std::string& msg, const IndexRange& loc)
			: Error(msg), m_loc(loc)
		{ }

		ParseError(const ParseError&) = default;
		ParseError& operator=(const ParseError&) = default;



		/** @{ */
		/**
		 * @brief Get a location of the character sequence that caused the parse to fail.
		*/
		const IndexRange& loc() const noexcept {
			return m_loc;
		}
		/** @} */



	private:
		IndexRange m_loc;
	};

}

#endif