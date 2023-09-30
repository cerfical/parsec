#ifndef PARSEC_GRAMMAR_HEADER
#define PARSEC_GRAMMAR_HEADER

#include "RegExNodes.hpp"

#include <vector>
#include <string>

namespace parsec {
	/**
	 * @brief Defines a pattern for forming a single instance of a named language token.
	 */
	class TokenPattern {
	public:
		/** @{ */
		/** @brief Construct an empty TokenPattern that defines nothing. */
		TokenPattern() = default;

		/** @brief Compile a TokenPattern from a token name, string pattern and priority. */
		TokenPattern(const std::string& name, const std::string& pattern, int priority = 0);

		/** @brief Construct a TokenPattern from a token name, a pattern compiled to a regex, and a priority. */
		TokenPattern(const std::string& name, std::unique_ptr<RegExNode> regex, int priority = 0);
		/** @} */

		/** @{ */
		TokenPattern(TokenPattern&&) = default;
		TokenPattern& operator=(TokenPattern&&) = default;
		/** @} */

		/** @{ */
		TokenPattern(const TokenPattern&) = delete;
		TokenPattern& operator=(const TokenPattern&) = delete;
		/** @} */

		/** @{ */
		/** @brief Name of the token that the pattern belongs to. */
		const std::string& GetName() const noexcept {
			return m_name;
		}

		/** @brief Regular expression that defines the pattern. */
		const RegExNode* GetRegex() const noexcept {
			return m_regex.get();
		}

		/** @brief An integer value specifying the relative precedence of similar patterns. */
		int GetPriority() const noexcept {
			return m_priority;
		}

		/** @brief Special position in the pattern regex that identifies the pattern's end. */
		const RegExChar* GetEndMarker() const noexcept {
			return static_cast<const RegExChar*>(m_regex->GetRightExpr());
		}
		/** @} */

		/** @{ */
		/** @brief String representation of the pattern. */
		std::string ToStr() const;
		/** @} */

	private:
		std::unique_ptr<RegExConcat> m_regex;
		std::string m_name;
		int m_priority = 0;
	};

	/** @brief List of @ref TokenPattern "TokenPatterns". */
	using TokenPatternList = std::vector<TokenPattern>;



	/**
	 * @brief Defines a syntax for some language.
	 */
	class Grammar {
	public:
		/** @{ */
		/** @brief Construct an empty Grammar that defines nothing. */
		Grammar() = default;
		/** @} */

		/** @{ */
		Grammar(Grammar&&) = default;
		Grammar& operator=(Grammar&&) = default;
		/** @} */

		/** @{ */
		Grammar(const Grammar&) = delete;
		Grammar& operator=(const Grammar&) = delete;
		/** @} */

		/** @{ */
		/** @brief Add a new token to the language. */
		template <typename... Args>
			requires std::constructible_from<TokenPattern, Args...>
		void AddToken(Args&&... args) {
			m_tokens.emplace_back(std::forward<Args>(args)...);
		}
		/** @} */

		/** @{ */
		/** @brief List of tokens defined by the language. */
		const TokenPatternList& GetTokens() const noexcept {
			return m_tokens;
		}

		/** @brief Number of tokens in the language. */
		std::size_t GetTokenCount() const noexcept {
			return m_tokens.size();
		}
		/** @} */

	private:
		TokenPatternList m_tokens;
	};
}

#endif