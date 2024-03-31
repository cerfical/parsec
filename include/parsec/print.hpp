#ifndef PARSEC_PRINT_HEADER
#define PARSEC_PRINT_HEADER

#include <ostream>

namespace parsec {
	namespace pars {
		enum class TokenKinds;
		class Token;
	}


	/**
	 * @name Printing objects to streams
	 * @{
	*/
	std::ostream& operator<<(std::ostream& out, pars::TokenKinds tok);
	
	std::ostream& operator<<(std::ostream& out, const pars::Token& tok);
	/** @} */

}

#endif