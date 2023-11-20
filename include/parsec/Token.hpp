#ifndef PARSEC_TOKEN_HEADER
#define PARSEC_TOKEN_HEADER

#include "TokenKinds.hpp"
#include "TokenBase.hpp"

namespace parsec {
	class Token : public TokenBase<TokenKinds> {
	public:
		/** @{ */
		static std::string_view describe(TokenKinds tok);
		/** @} */

		/** @{ */
		using TokenBase::TokenBase;
		/** @} */
	};
}

#endif