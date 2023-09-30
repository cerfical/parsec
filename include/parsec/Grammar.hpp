#ifndef PARSEC_GRAMMAR_HEADER
#define PARSEC_GRAMMAR_HEADER

#include <string>
#include <vector>

namespace parsec {
	/**
	 * @brief Defines a language by means of some syntax rules and tokens.
	 */
	class Grammar {
	public:
		/**
		 * @brief Describes a token from a language grammar.
		 */
		class Token {
		public:
			/** @{ */
			/** @brief Construct a token description from a name and its defining regular expression. */
			Token(const std::string& name, const std::string& regex)
			 : name(name), regex(regex)
			{ }

			/** @copybrief */
			~Token() = default;
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
			/** @brief Get the name of the token. */
			const std::string& GetName() const noexcept {
				return name;
			}

			/** @brief Get the regular expression that defines the token. */
			const std::string& GetRegex() const noexcept {
				return regex;
			}
			/** @} */

		private:
			std::string name;
			std::string regex;
		};

		/** @{ */
		/** @copybrief */
		Grammar() = default;

		/** @copybrief */
		~Grammar() = default;
		/** @} */

		/** @{ */
		Grammar(const Grammar&) = default;
		Grammar& operator=(const Grammar&) = default;
		/** @} */

		/** @{ */
		Grammar(Grammar&&) = default;
		Grammar& operator=(Grammar&&) = default;
		/** @} */

		/** @{ */
		/** @brief Add a new token to the grammar. */
		void AddToken(const std::string& name, const std::string& regex) {
			tokens.emplace_back(name, regex);
		}
		/** @} */

		/** @{ */
		/** @brief Get the list of available tokens for the language. */
		const auto& GetTokens() const noexcept {
			return tokens;
		}
		/** @} */

	private:
		std::vector<Token> tokens;
	};
}

#endif