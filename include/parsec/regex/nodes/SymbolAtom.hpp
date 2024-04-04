#ifndef PARSEC_REGEX_NODES_SYMBOL_ATOM_HEADER
#define PARSEC_REGEX_NODES_SYMBOL_ATOM_HEADER

#include "../../core/Symbol.hpp"
#include "ExprNode.hpp"

namespace parsec::regex {

	class SymbolAtom : public ExprNode {
	public:
		
		explicit SymbolAtom(const Symbol& value)
			: m_value(value) {}

		~SymbolAtom() override = default;


		void accept(NodeVisitor& visitor) const override;

		bool isNullable() const noexcept override {
			return m_value.isEmpty();
		}

		int atomCount() const noexcept override {
			return m_value ? 1 : 0;
		}


		const Symbol& value() const {
			return m_value;
		}


	private:
		Symbol m_value;
	};

}

#endif