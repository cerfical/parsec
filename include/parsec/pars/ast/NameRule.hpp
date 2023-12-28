#ifndef PARSEC_PARS_AST_NAME_RULE_HEADER
#define PARSEC_PARS_AST_NAME_RULE_HEADER

#include "../Token.hpp"
#include "Node.hpp"

namespace parsec::pars::ast {

	/**
	 * @brief Simple rule consisting of a single symbolic name.
	*/
	class NameRule : public Node {
	public:

		explicit NameRule(Token name)
			: name(std::move(name))
		{ }

		~NameRule() override = default;


		void acceptVisitor(NodeVisitor& visitor) const override;


		Token name;
	};

}

#endif