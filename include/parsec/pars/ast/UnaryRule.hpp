#ifndef PARSEC_PARS_AST_UNARY_RULE_HEADER
#define PARSEC_PARS_AST_UNARY_RULE_HEADER

#include "Node.hpp"

namespace parsec::pars::ast {

	/**
	 * @brief Rule consisting of one subrule.
	*/
	class UnaryRule : public Node {
	public:

		explicit UnaryRule(NodePtr inner)
			: m_inner(std::move(inner)) {}

		~UnaryRule() override = default;


		/**
		 * @brief Inner subrule.
		*/
		const Node* inner() const {
			return m_inner.get();
		}


	private:
		NodePtr m_inner;
	};

}

#endif