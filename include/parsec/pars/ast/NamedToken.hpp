#ifndef PARSEC_PARS_AST_NAMED_TOKEN_HEADER
#define PARSEC_PARS_AST_NAMED_TOKEN_HEADER

#include "../Token.hpp"
#include "Node.hpp"

namespace parsec::pars::ast {

	class NamedToken : public Node {
	public:

		NamedToken(const Token& name, const Token& pattern)
			: name(name), pattern(pattern) {}

		~NamedToken() override = default;


		void acceptVisitor(NodeVisitor& visitor) const override;


		Token name;
		Token pattern;
	};

}

#endif