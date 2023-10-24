#ifndef PARSEC_FG_RULES_CHAR_LITERAL_HEADER
#define PARSEC_FG_RULES_CHAR_LITERAL_HEADER

#include "RulePos.hpp"

namespace parsec::fg::rules {
	class CharLiteral : public RulePos {
	public:
		/** @{ */
		explicit CharLiteral(char value) noexcept
			: m_value(value)
		{ }
		/** @} */


		/** @{ */
		void traverseWith(RuleTraverser& traverser) const override;
		/** @} */


		/** @{ */
		char value() const noexcept {
			return m_value;
		}
		/** @} */


	private:
		/** @{ */
		char m_value = '\0';
		/** @} */
	};
}

#endif