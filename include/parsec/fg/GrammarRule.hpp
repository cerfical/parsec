#ifndef PARSEC_FG_GRAMMAR_RULE_HEADER
#define PARSEC_FG_GRAMMAR_RULE_HEADER

#include <iostream>

namespace parsec::fg {
	/**
	 * @brief Named inference rule for generating grammar symbols.
	 */
	class GrammarRule {
	public:
		/** @{ */
		/** @brief Output stream operator for a rule. */
		friend std::ostream& operator<<(std::ostream& out, const GrammarRule& r) {
			r.print(out);
			return out;
		}
		/** @} */


		/** @{ */
		/** @brief Create a new named rule. */
		explicit GrammarRule(const std::string& name)
			: m_name(name)
		{ }


		/** @brief Destroy the rule. */
		virtual ~GrammarRule() = default;
		/** @} */


		/** @{ */
		GrammarRule(GrammarRule&&) = default;
		GrammarRule& operator=(GrammarRule&&) = default;
		/** @} */


		/** @{ */
		GrammarRule(const GrammarRule&) = delete;
		GrammarRule& operator=(const GrammarRule&) = delete;
		/** @} */


		/** @{ */
		/** @brief Prints out the rule to a @c std::ostream. */
		virtual void print(std::ostream& out = std::cout) const = 0;
		/** @} */


		/** @{ */
		/** @brief Name of the rule. */
		const std::string& name() const noexcept {
			return m_name;
		}
		/** @} */


	private:
		std::string m_name;
	};
}

#endif