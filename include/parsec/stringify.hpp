#ifndef PARSEC_STRINGIFY_HEADER
#define PARSEC_STRINGIFY_HEADER

#include <string>

namespace parsec {
	namespace pars {
		enum class TokenKinds;
		class Token;
	}


	/**
	 * @name Converting objects to strings
	 * @{
	*/
	std::string stringify(pars::TokenKinds tok);
	
	std::string stringify(const pars::Token& tok);
	/** @} */

}

#endif