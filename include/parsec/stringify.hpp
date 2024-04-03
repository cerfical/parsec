#ifndef PARSEC_STRINGIFY_HEADER
#define PARSEC_STRINGIFY_HEADER

#include <string>

namespace parsec {
	
	class RegularExpr;

	namespace pars {
		enum class TokenKinds;
		class Token;
	}

	namespace regex {
		class ExprNode;
	}


	/**
	 * @name Converting objects to strings
	 * @{
	*/
	std::string stringify(const RegularExpr& expr);

	std::string stringify(pars::TokenKinds tok);
	
	std::string stringify(const pars::Token& tok);

	std::string stringify(const regex::ExprNode& n);
	/** @} */

}

#endif