#ifndef PARSEC_LEX_GRAMMAR_HEADER
#define PARSEC_LEX_GRAMMAR_HEADER

#include <initializer_list>
#include <string>
#include <vector>

namespace parsec {
	/**
	 * @brief Definition of a single token from a language.
	 */
	class LexToken {
	public:
		/** @{ */
		/** @brief Construct a new LexToken defined by the specified name and regex. */
		LexToken(const std::string& name, const std::string& regex)
		 : name(name), regex(regex)
		{ }

		/** @copybrief */
		~LexToken() = default;
		/** @} */

		/** @{ */
		LexToken(const LexToken&) = default;
		LexToken& operator=(const LexToken&) = default;
		/** @} */

		/** @{ */
		LexToken(LexToken&&) = default;
		LexToken& operator=(LexToken&&) = default;
		/** @} */

		/** @{ */
		/** @brief Get the name of the token. */
		const std::string& GetName() const noexcept {
			return name;
		}

		/** @brief Get the regular expression that defines the character sequences representing the token. */
		const std::string& GetRegex() const noexcept {
			return regex;
		}
		/** @} */

	private:
		std::string name;
		std::string regex;
	};

	/** @brief List of @ref LexToken "LexTokens". */
	using LexTokenList = std::vector<LexToken>;



	/**
	 * @brief Defines rules for forming lexical units or tokens of some language.
	 */
	class LexGrammar {
	public:
		/** @{ */
		/** @copybrief */
		LexGrammar() = default;

		/** @brief Construct a new LexGrammar from a list of @ref LexToken "LexTokens". */
		LexGrammar(std::initializer_list<LexToken> tokens)
		 : tokens(std::move(tokens))
		{ }

		/** @copybrief */
		~LexGrammar() = default;
		/** @} */

		/** @{ */
		LexGrammar(const LexGrammar&) = default;
		LexGrammar& operator=(const LexGrammar&) = default;
		/** @} */

		/** @{ */
		LexGrammar(LexGrammar&&) = default;
		LexGrammar& operator=(LexGrammar&&) = default;
		/** @} */

		/** @{ */
		/** @brief Add a new token definition to the grammar. */
		void AddToken(const std::string& name, const std::string& regex) {
			tokens.emplace_back(name, regex);
		}
		/** @} */

		/** @{ */
		/** @brief Get the list of tokens defined by the grammar. */
		const LexTokenList& GetTokens() const noexcept {
			return tokens;
		}

		/** @brief Get the number of tokens in the grammar. */
		std::size_t GetTokensCount() const noexcept {
			return tokens.size();
		}
		/** @} */

	private:
		LexTokenList tokens;
	};
}

#endif