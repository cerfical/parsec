#ifndef PARSEC_PARS_AST_INLINE_TOKEN_HEADER
#define PARSEC_PARS_AST_INLINE_TOKEN_HEADER

#include "../Token.hpp"
#include "Node.hpp"

namespace parsec::pars::ast {

	class InlineToken : public Node {
	public:

		explicit InlineToken(const Token& pattern)
			: pattern(pattern) {}

		~InlineToken() override = default;


		void acceptVisitor(NodeVisitor& visitor) const override;


		Token pattern;
	};

}

#endif