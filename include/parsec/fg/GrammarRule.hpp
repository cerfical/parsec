#ifndef PARSEC_FG_GRAMMAR_RULE_HEADER
#define PARSEC_FG_GRAMMAR_RULE_HEADER

#include <string>

namespace parsec::fg {
	/**
	 * @brief Represents general grammatical constructs.
	 */
	class GrammarRule {
	public:
		/** @{ */
		GrammarRule() = default;

		/** @brief Create a named rule. */
		explicit GrammarRule(const std::string& name)
			: m_name(name)
		{ }
		/** @} */


		/** @{ */
		/** @brief Name of the rule. */
		const std::string& name() const noexcept {
			return m_name;
		}
		/** @} */


	protected:
		/** @{ */
		virtual ~GrammarRule() = 0;
		/** @} */


	private:
		/** @{ */
		std::string m_name;
		/** @} */
	};

	inline GrammarRule::~GrammarRule() = default;
}

#endif