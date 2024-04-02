#ifndef PARSEC_REGEX_SYMBOL_ATOM_HEADER
#define PARSEC_REGEX_SYMBOL_ATOM_HEADER

#include "../core/Symbol.hpp"
#include "ExprNode.hpp"

namespace parsec::regex {

	class SymbolAtom : public ExprNode {
	public:
		
		explicit SymbolAtom(const Symbol& symbol)
			: m_symbol(symbol) {}

		~SymbolAtom() override = default;


		void accept(NodeVisitor& visitor) const override;

		bool isNullable() const noexcept override {
			return m_symbol.isEmpty();
		}


		const Symbol& symbol() const {
			return m_symbol;
		}


	private:
		Symbol m_symbol;
	};

}

#endif