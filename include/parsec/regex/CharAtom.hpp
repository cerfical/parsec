#ifndef PARSEC_REGEX_CHAR_ATOM_HEADER
#define PARSEC_REGEX_CHAR_ATOM_HEADER

#include "ExprNode.hpp"

namespace parsec::regex {
	
	/**
	 * @brief Character literal.
	 */
	class CharAtom : public ExprNode {
	public:
		
		explicit CharAtom(char ch) noexcept
			: m_value(ch)
		{ }

		~CharAtom() override = default;


		void acceptVisitor(NodeVisitor& visitor) const override;

		int atomCount() const noexcept override {
			return 1;
		}


		/** @brief Positional index of the atom in the enclosing regular expression. */
		int posIndex() const noexcept {
			return m_posIndex;
		}

		/** @brief Value of the character literal. */
		char value() const noexcept {
			return m_value;
		}


	private:
		void rebaseAtomIndices(int base) noexcept override {
			m_posIndex = base;
		}

		int m_posIndex = 0;
		char m_value;
	};

}

#endif