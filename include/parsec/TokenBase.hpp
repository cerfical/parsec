#ifndef PARSEC_TOKEN_BASE_HEADER
#define PARSEC_TOKEN_BASE_HEADER

#include "SourceLoc.hpp"

#include <string>
#include <ostream>

namespace parsec {
	template <typename TokenKinds>
	class TokenBase {
	public:
		/** @{ */
		friend std::ostream& operator<<(std::ostream& out, const TokenBase& tok) {
			out << "{ " << tok.kind() << ": \"" << tok.text() << "\" }";
			return out;
		}
		/** @} */


		/** @{ */
		TokenBase() = default;

		TokenBase(
			const std::string& text,
			TokenKinds kind,
			const SourceLoc& loc = {}
		)
			: m_text(text)
			, m_loc(loc)
			, m_kind(kind)
		{ }
		/** @} */


		/** @{ */
		TokenBase(const TokenBase&) = default;
		TokenBase& operator=(const TokenBase&) = default;
		/** @} */

		/** @{ */
		TokenBase(TokenBase&&) = default;
		TokenBase& operator=(TokenBase&&) = default;
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