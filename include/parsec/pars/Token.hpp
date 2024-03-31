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

		friend std::ostream& operator<<(std::ostream& out, const Token& tok);



		Token() = default;

		Token(const std::string& text, TokenKinds kind, const SourceLoc& loc)
			: m_text(text), m_loc(loc), m_kind(kind) {}



		/** @{ */
		Token(const Token&) = default;
		Token& operator=(const Token&) = default;
		
		Token(Token&&) = default;
		Token& operator=(Token&&) = default;
		/** @} */



		/** @{ */
		/**
		 * @brief Character sequence enclosed by the token.
		*/
		const std::string& text() const noexcept {
			return m_text;
		}



		/**
		 * @brief Location of the token.
		*/
		const SourceLoc& loc() const noexcept {
			return m_loc;
		}


		/**
		 * @brief Classification of the token.
		*/
		TokenKinds kind() const noexcept {
			return m_kind;
		}



		/**
		 * @brief Check if the token is of the specified type.
		*/
		bool is(TokenKinds kind) const noexcept {
			return this->kind() == kind;
		}



		/**
		 * @brief Check if the token is an end-of-file token.
		*/
		bool isEof() const noexcept {
			return is(TokenKinds::Eof);
		}
		/** @} */



	private:
		std::string m_text;
		SourceLoc m_loc;
		TokenKinds m_kind = {};
	};

}

#endif