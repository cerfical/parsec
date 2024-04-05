#ifndef PARSEC_PARS_AST_SYMBOL_ATOM_HEADER
#define PARSEC_PARS_AST_SYMBOL_ATOM_HEADER

#include "../Token.hpp"
#include "Node.hpp"

namespace parsec::pars {

	/**
	 * @brief Trivial rule consisting of a single symbolic name.
	*/
	class SymbolAtom : public Node {
	public:

		explicit SymbolAtom(const Token& value)
			: m_value(value) {}

		~SymbolAtom() override = default;


		void accept(NodeVisitor& visitor) const override;


		/**
		 * @brief Value of the symbolic name.
		*/
		const Token& value() const {
			return m_value;
		}


	private:
		Token m_value;
	};

}

#endif