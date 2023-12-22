#ifndef PARSEC_CORE_EOF_ERROR_HEADER
#define PARSEC_CORE_EOF_ERROR_HEADER

#include "Error.hpp"
#include <gsl/util>

namespace parsec {

	/**
	 * @brief Thrown by a TextScanner when the file end is unexpectedly reached.
	*/
	class EofError : public Error {
	public:
		
		explicit EofError(gsl::index loc)
			: Error("unexpected end of file"), m_loc(loc)
		{ }
		
		EofError(const EofError&) = default;
		EofError& operator=(const EofError&) = default;



		/** @{ */
		/**
		 * @brief Get the position of a scanner in an input file when the error occurred.
		*/
		gsl::index loc() const noexcept {
			return m_loc;
		}
		/** @} */



	private:
		gsl::index m_loc;
	};

}

#endif