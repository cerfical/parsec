#ifndef PARSEC_REGEX_NODES_CHAR_ATOM_HEADER
#define PARSEC_REGEX_NODES_CHAR_ATOM_HEADER

#include "ExprNode.hpp"

namespace parsec::regex::nodes {
	
	/**
	 * @brief Character literal.
	*/
	class CharAtom : public ExprNode {
	public:
		
		CharAtom() = default;

		explicit CharAtom(char value) noexcept
			: value(value)
		{ }

		~CharAtom() override = default;


		void acceptVisitor(NodeVisitor& visitor) const override;


		/** @brief Value of the character literal. */
		char value = {};

	};

}

#endif