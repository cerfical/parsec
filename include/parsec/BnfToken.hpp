#ifndef PARSEC_BNF_TOKEN_HEADER
#define PARSEC_BNF_TOKEN_HEADER

#define PARSEC_BNF_TOKEN_LIST \
	PARSEC_BNF_TOKEN(Eof) /**< @brief End of file. */ \
\
	PARSEC_BNF_TOKEN(Ident) /**< @brief Identifier. */ \
	PARSEC_BNF_TOKEN(Regex) /**< @brief Regular expression. */ \
\
	PARSEC_BNF_TOKEN(Pipe) /**< @brief Vertical bar. */ \
	PARSEC_BNF_TOKEN(Semicolon) /**< @brief Semicolon. */ \
	PARSEC_BNF_TOKEN(Equals) /**< @brief Equals sign. */ \
\
	PARSEC_BNF_TOKEN(OpenBrace) /**< @brief Opening brace. */ \
	PARSEC_BNF_TOKEN(CloseBrace) /**< @brief Closing brace. */

#include "Utils.hpp"

#include <string>
#include <ostream>

namespace parsec {
#define PARSEC_BNF_TOKEN(tok) tok,
	
	/**
	 * @brief Defines available type codes for classifying @ref parsec::BnfToken "BnfTokens".
	 */
	enum class BnfTokenKinds {
		PARSEC_BNF_TOKEN_LIST
	};

#undef PARSEC_BNF_TOKEN

	/** @brief Print a @ref BnfTokenKinds enumerator to a @c std::ostream. */
	std::ostream& operator<<(std::ostream& out, BnfTokenKinds tok);



	/**
	 * @brief Description of a single character sequence from a BNF-like grammar definition.
	 */
	class BnfToken {
	public:
		/** @brief Print out a BnfToken to a @c std::ostream. */
		friend std::ostream& operator<<(std::ostream& out, const BnfToken& tok);

		/** @{ */
		/** @brief Construct an empty @ref BnfTokenKinds::Eof "Eof" token. */
		BnfToken() = default;

		/** @brief Construct a new token from its textual representation, classification and location. */
		BnfToken(const std::string& text, BnfTokenKinds kind, const SourceLoc& loc)
		 : m_text(text), m_loc(loc), m_kind(kind)
		{ }
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
			return m_text;
		}

		/** @brief Get a location in the source code where the token was found. */
		const SourceLoc& GetLocation() const noexcept {
			return m_loc;
		}

		/** @brief Get the classification of the token as reported by BnfLexer. */
		BnfTokenKinds GetKind() const noexcept {
			return m_kind;
		}

		/** @brief Check if the token is an @ref BnfTokenKinds::Eof "Eof" token. */
		bool IsEof() const noexcept {
			return GetKind() == BnfTokenKinds::Eof;
		}
		/** @} */

	private:
		std::string m_text;
		SourceLoc m_loc;
		BnfTokenKinds m_kind = BnfTokenKinds::Eof;
	};
}

#endif