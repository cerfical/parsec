#ifndef PARSEC_PRINT_HEADER
#define PARSEC_PRINT_HEADER

#include "stringify.hpp"
#include <ostream>

namespace parsec {
	namespace pars {
		enum class TokenKinds;
		class Token;
	}

	namespace regex {
		class ExprNode;
	}


	/**
	 * @name Printing objects to streams
	 * @{
	*/
	inline std::ostream& operator<<(std::ostream& out, pars::TokenKinds tok) {
		return out << stringify(tok);
	}
	
	inline std::ostream& operator<<(std::ostream& out, const pars::Token& tok) {
		return out << stringify(tok);
	}

	inline std::ostream& operator<<(std::ostream& out, const regex::ExprNode& n) {
		return out << stringify(n);
	}
	/** @} */

}

#endif