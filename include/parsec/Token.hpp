#ifndef PARSEC_TOKEN_HEADER
#define PARSEC_TOKEN_HEADER

#define PARSEC_TOKEN_LIST \
	PARSEC_TOKEN(Eof, "eof") /**< @brief End of file. */ \
\
	PARSEC_TOKEN(Ident, "ident") /**< @brief Identifier. */ \
	PARSEC_TOKEN(Regex, "regex") /**< @brief Regular expression. */ \
\
	PARSEC_TOKEN(Pipe, "|") /**< @brief Vertical bar. */ \
	PARSEC_TOKEN(Semicolon, ";") /**< @brief Semicolon. */ \
	PARSEC_TOKEN(Equals, "=") /**< @brief Equals sign. */ \
\
	PARSEC_TOKEN(LeftBrace, "{") /**< @brief Left brace. */ \
	PARSEC_TOKEN(RightBrace, "}") /**< @brief Right brace. */

#include "Utils.hpp"

#include <string_view>
#include <string>

namespace parsec {
#define PARSEC_TOKEN(kind, text) kind,
	/**
	 * @brief Enumerates available type codes for classifying tokens.
	 */
	enum class TokenKinds {
		PARSEC_TOKEN_LIST
	};
#undef PARSEC_TOKEN

	/**
	 * @brief Character string classified accordingly by the Lexer.
	 */
	class Token {
	public:
		/** @{ */
		/** @brief Construct an empty @ref TokenKinds::Eof "Eof" Token. */
		Token() = default;

		/** @brief Construct a Token from its textual representation, classification and location. */
		Token(std::string_view text, TokenKinds kind, const SourceLocation& loc)
		 : loc(loc), text(text), kind(kind)
		{ }

		/** @brief Construct a Token from its textual representation and classification, with no location attached. */
		Token(std::string_view text, TokenKinds kind)
		 : Token(text, kind, { })
		{ }

		/** @brief Destroy Token. */
		~Token() = default;
		/** @} */

		/** @{ */
		/** @brief Construct a Token from a copy of Token object. */
		Token(const Token&) = default;

		/** @brief Assign a copy of Token object to Token. */
		Token& operator=(const Token&) = default;
		/** @} */

		/** @{ */
		/** @brief Construct a Token from a moved copy of Token object. */
		Token(Token&&) = default;

		/** @brief Assign a moved copy of Token object to Token. */
		Token& operator=(Token&&) = default;
		/** @} */

		/** @{ */
		/** @brief Get textual representation of Token. */
		const std::string& GetText() const noexcept {
			return text;
		}

		/** @brief Get location in the source code where Token was found. */
		const SourceLocation& GetLocation() const noexcept {
			return loc;
		}

		/** @brief Get Token type as reported by Lexer. */
		TokenKinds GetKind() const noexcept {
			return kind;
		}

		/** @brief Check if Token is @ref TokenKinds::Eof "Eof" Token. */
		bool IsEof() const noexcept {
			return GetKind() == TokenKinds::Eof;
		}
		/** @} */

	private:
		SourceLocation loc;
		std::string text;
		TokenKinds kind = TokenKinds::Eof;
	};
}

#undef PARSEC_TOKEN_LIST
#endif