#ifndef PARSEC_PARS_AST_SYMBOL_RULE_HEADER
#define PARSEC_PARS_AST_SYMBOL_RULE_HEADER

#include "../Token.hpp"
#include "Node.hpp"

namespace parsec::pars::ast {

	/**
	 * @brief Trivial rule that matches a single symbolic name.
	*/
	class SymbolRule : public Node {
	public:

		explicit SymbolRule(const Token& symbol)
			: m_symbol(symbol) {}

		~SymbolRule() override = default;


		void acceptVisitor(NodeVisitor& visitor) const override;


		/**
		 * @brief Symbolic name to match.
		*/
		const Token& symbol() const {
			return m_symbol;
		}


	private:
		Token m_symbol;
	};

}

#endif