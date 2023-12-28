#ifndef PARSEC_PARS_AST_NAMED_RULE_HEADER
#define PARSEC_PARS_AST_NAMED_RULE_HEADER

#include "../Token.hpp"
#include "Node.hpp"

namespace parsec::pars::ast {

	/**
	 * @brief Represents a rule with a name.
	*/
	class NamedRule : public Node {
	public:
			
		NamedRule(Token name, NodePtr rule)
			: name(std::move(name)), rule(std::move(rule))
		{ }

		~NamedRule() override = default;


		void acceptVisitor(NodeVisitor& visitor) const override;


		Token name;
		NodePtr rule;
	};

}

#endif