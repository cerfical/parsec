#ifndef PARSEC_FG_TOKEN_HEADER
#define PARSEC_FG_TOKEN_HEADER

#include "../utils/SourceLoc.hpp"
#include "TokenKinds.hpp"

#include <string>
#include <ostream>

namespace parsec::fg {
	class Token {
	public:
		/** @{ */
		friend std::ostream& operator<<(std::ostream& out, const Token& tok);
		/** @{ */


		/** @{ */
		Token(
			const std::string& text = "",
			TokenKinds kind = TokenKinds::Eof,
			const SourceLoc& loc = {}
		)
			: m_text(text)
			, m_loc(loc)
			, m_kind(kind)
		{ }
		/** @} */


		/** @{ */
		Token(const Token&) = default;
		Token& operator=(const Token&) = default;
		/** @} */

		/** @{ */
		Token(Token&&) = default;
		Token& operator=(Token&&) = default;
		/** @} */


		/** @{ */
		const std::string& text() const noexcept {
			return m_text;
		}

		const SourceLoc& loc() const noexcept {
			return m_loc;
		}

		TokenKinds kind() const noexcept {
			return m_kind;
		}
		/** @} */


		/** @{ */
		bool is(TokenKinds kind) const noexcept {
			return m_kind == kind;
		}

		bool eof() const noexcept {
			return is(TokenKinds::Eof);
		}
		/** @} */


	private:
		/** @{ */
		std::string m_text;
		SourceLoc m_loc;
		TokenKinds m_kind;
		/** @} */
	};
}

#endif