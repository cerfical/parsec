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
	PARSEC_BNF_TOKEN(LeftBrace, "{") /**< @brief Left brace. */ \
	PARSEC_BNF_TOKEN(RightBrace, "}") /**< @brief Right brace. */

#include "Utils.hpp"

#include <string_view>
#include <string>

namespace parsec {
#define PARSEC_BNF_TOKEN(kind, text) kind,
	/**
	 * @brief Defines available type codes for classifying @ref parsec::BnfToken "BnfToken"s.
	 */
	enum class BnfTokenKinds {
		PARSEC_BNF_TOKEN_LIST
	};
#undef PARSEC_BNF_TOKEN

	/**
	 * @brief A string of characters representing a single word from the BNF language.
	 */
	class BnfToken {
	public:
		/** @{ */
		/** @brief Construct a new empty @ref BnfTokenKinds::Eof "Eof" BnfToken. */
		BnfToken() = default;

		/** @brief Construct a new BnfToken from its textual representation, classification and location. */
		BnfToken(const std::string& text, BnfTokenKinds kind, const SourceLocation& loc)
		 : loc(loc), text(text), kind(kind)
		{ }

		/** @copybrief BnfToken(const std::string&, BnfTokenKinds, const SourceLocation&) */
		BnfToken(std::string&& text, BnfTokenKinds kind, const SourceLocation& loc)
		 : loc(loc), text(std::move(text)), kind(kind)
		{ }

		/** @brief Destroy the BnfToken. */
		~BnfToken() = default;
		/** @} */

		/** @{ */
		/** @brief Construct a new BnfToken by making a copy of another BnfToken. */
		BnfToken(const BnfToken&) = default;

		/** @brief Assign to the BnfToken a copy of another BnfToken. */
		BnfToken& operator=(const BnfToken&) = default;
		/** @} */

		/** @{ */
		/** @brief Construct a new BnfToken by moving from another BnfToken. */
		BnfToken(BnfToken&&) = default;

		/** @brief Move another BnfToken into the BnfToken. */
		BnfToken& operator=(BnfToken&&) = default;
		/** @} */

		/** @{ */
		/** @brief Get the textual representation of the BnfToken. */
		const std::string& GetText() const noexcept {
			return text;
		}

		/** @brief Get a location in the source code where the BnfToken was found. */
		const SourceLocation& GetLocation() const noexcept {
			return loc;
		}

		/** @brief Get the classification of the BnfToken as reported by BnfLexer. */
		BnfTokenKinds GetKind() const noexcept {
			return kind;
		}

		/** @brief Check if the BnfToken is an @ref BnfTokenKinds::Eof "Eof" BnfToken. */
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