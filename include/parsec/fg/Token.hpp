#ifndef PARSEC_FG_TOKEN_HEADER
#define PARSEC_FG_TOKEN_HEADER

#define PARSEC_FG_TOKEN_LIST \
	PARSEC_FG_TOKEN(Eof) /**< @brief End of file. */ \
\
	PARSEC_FG_TOKEN(Ident) /**< @brief Identifier. */ \
	PARSEC_FG_TOKEN(StringLiteral) /**< @brief String literal. */ \
\
	PARSEC_FG_TOKEN(Pipe) /**< @brief Vertical bar. */ \
	PARSEC_FG_TOKEN(Semicolon) /**< @brief Semicolon. */ \
	PARSEC_FG_TOKEN(Equals) /**< @brief Equals sign. */ \
\
	PARSEC_FG_TOKEN(OpenBrace) /**< @brief Opening brace. */ \
	PARSEC_FG_TOKEN(CloseBrace) /**< @brief Closing brace. */

#include "../utils/SourceLoc.hpp"

#include <string>
#include <ostream>

namespace parsec::fg {
	/**
	 * @brief Description of a single character sequence from a grammar definition.
	 */
	class Token {
	public:

#define PARSEC_FG_TOKEN(tok) tok,

		/**
		 * @brief Defines available type codes for classifying tokens.
		 */
		enum Kinds {
			PARSEC_FG_TOKEN_LIST
		};

#undef PARSEC_FG_TOKEN


		/** @brief Print out a token to a @c std::ostream. */
		friend std::ostream& operator<<(std::ostream& out, const Token& tok);

		/** @brief Print out a token kind to a @c std::ostream. */
		friend std::ostream& operator<<(std::ostream& out, Kinds kind);


		/** @{ */
		/** @brief Construct an empty Kinds::Eof token. */
		Token() = default;

		/** @brief Construct a new token from its classification, textual representation and location. */
		Token(Kinds kind, const std::string& text, const SourceLoc& loc)
			: m_text(text), m_loc(loc), m_kind(kind)
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
		/** @brief Textual representation of the token. */
		const std::string& text() const noexcept {
			return m_text;
		}

		/** @brief Location in the source code where the token was found. */
		const SourceLoc& location() const noexcept {
			return m_loc;
		}

		/** @brief Classification of the token as reported by some Lexer. */
		Kinds kind() const noexcept {
			return m_kind;
		}

		/** @brief Check if the token is of the specified type. */
		template <Kinds kind>
		bool is() const noexcept {
			return this->kind() == kind;
		}

		/** @brief Check if the token is a Kinds::Eof token. */
		bool eof() const noexcept {
			return is<Eof>();
		}
		/** @} */


		/** @{ */
		/** @brief Retrieve the string representation of the token. */
		std::string toStr() const;
		/** @} */

	private:
		std::string m_text;
		SourceLoc m_loc;
		Kinds m_kind = Eof;
	};
}

#endif