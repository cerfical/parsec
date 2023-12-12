#ifndef PARSEC_REGEX_CHAR_ATOM_HEADER
#define PARSEC_REGEX_CHAR_ATOM_HEADER

#include "ExprNode.hpp"

namespace parsec::regex {
	/**
	 * @brief Character literal node.
	 */
	class CharAtom : public ExprNode {
	public:
		explicit CharAtom(char ch)
			: m_value(ch)
		{ }


		CharAtom(CharAtom&&) = default;
		CharAtom& operator=(CharAtom&&) = default;

		~CharAtom() override = default;


		/** @{ */
		void acceptVisitor(NodeVisitor& visitor) const override;
		
		char value() const noexcept {
			return m_value;
		}
		/** @} */

	private:
		char m_value;
	};
}

#endif