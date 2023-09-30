#ifndef PARSEC_BNF_TOKEN_HEADER
#define PARSEC_BNF_TOKEN_HEADER

#define PARSEC_BNF_TOKEN_LIST \
	PARSEC_BNF_TOKEN(Eof, "eof") /**< @brief End of file. */ \
\
	PARSEC_BNF_TOKEN(Ident, "ident") /**< @brief Identifier. */ \
	PARSEC_BNF_TOKEN(Regex, "regex") /**< @brief Regular expression. */ \
\
	PARSEC_BNF_TOKEN(Pipe, "|") /**< @brief Vertical bar. */ \
	PARSEC_BNF_TOKEN(Semicolon, ";") /**< @brief Semicolon. */ \
	PARSEC_BNF_TOKEN(Equals, "=") /**< @brief Equals sign. */ \
\
	PARSEC_BNF_TOKEN(OpenBrace, "{") /**< @brief Opening brace. */ \
	PARSEC_BNF_TOKEN(CloseBrace, "}") /**< @brief Closing brace. */

#include "Utils.hpp"
#include <string>

namespace parsec {
#define PARSEC_BNF_TOKEN(kind, text) kind,
	/**
	 * @brief Defines available type codes for classifying @ref parsec::BnfToken "BnfTokens".
	 */
	enum class BnfTokenKinds {
		PARSEC_BNF_TOKEN_LIST
	};
#undef PARSEC_BNF_TOKEN

	/**
	 * @brief Description and classification of some character sequence from a language definition in the BNF-like form.
	 */
	class BnfToken {
	public:
		/** @{ */
		/** @brief Construct a new empty @ref BnfTokenKinds::Eof "Eof" token. */
		BnfToken() = default;

		/** @brief Construct a new token from its textual representation, classification and location. */
		BnfToken(const std::string& text, BnfTokenKinds kind, const SourceLocation& loc)
		 : loc(loc), text(text), kind(kind)
		{ }

		/** @copybrief */
		~BnfToken() = default;
		/** @} */

		/** @{ */
		BnfToken(const BnfToken&) = default;
		BnfToken& operator=(const BnfToken&) = default;
		/** @} */

		/** @{ */
		BnfToken(BnfToken&&) = default;
		BnfToken& operator=(BnfToken&&) = default;
		/** @} */

		/** @{ */
		/** @brief Get the textual representation of the token. */
		const std::string& GetText() const noexcept {
			return text;
		}

		/** @brief Get a location in the source code where the token was found. */
		const SourceLocation& GetLocation() const noexcept {
			return loc;
		}

		/** @brief Get the classification of the token as reported by BnfLexer. */
		BnfTokenKinds GetKind() const noexcept {
			return kind;
		}

		/** @brief Check if the token is an @ref BnfTokenKinds::Eof "Eof" token. */
		bool IsEof() const noexcept {
			return GetKind() == BnfTokenKinds::Eof;
		}
		/** @} */

	private:
		SourceLocation loc;
		std::string text;
		BnfTokenKinds kind = BnfTokenKinds::Eof;
	};
}

#undef PARSEC_BNF_TOKEN_LIST
#endif