#ifndef PARSEC_PARS_TOKEN_HEADER
#define PARSEC_PARS_TOKEN_HEADER

#include "../core/SourceLoc.hpp"
#include "TokenKinds.hpp"

#include <string>

namespace parsec::pars {

	/**
	 * @brief Character sequence classified according to some rules.
	*/
	class Token {
	public:

		Token() = default;

		Token(const std::string& text, TokenKinds kind, const SourceLoc& loc)
			: m_text(text), m_loc(loc), m_kind(kind) {}



		/** @{ */
		/**
		 * @brief Character sequence covered by the token.
		*/
		const std::string& text() const {
			return m_text;
		}



		/**
		 * @brief Location of the token.
		*/
		const SourceLoc& loc() const {
			return m_loc;
		}



		/**
		 * @brief Classification of the token.
		*/
		TokenKinds kind() const {
			return m_kind;
		}



		/**
		 * @brief Check if the token is of the specified type.
		*/
		template <TokenKinds tok>
		bool is() const {
			return kind() == tok;
		}
		/** @} */



	private:
		std::string m_text;
		SourceLoc m_loc;
		TokenKinds m_kind = {};
	};

}

#endif