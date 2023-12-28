#ifndef PARSEC_PARS_AST_NAMED_PATTERN_HEADER
#define PARSEC_PARS_AST_NAMED_PATTERN_HEADER

#include "../Token.hpp"
#include "Node.hpp"

namespace parsec::pars::ast {

	/**
	 * @brief Represents a string pattern with a name.
	*/
	class NamedPattern : public Node {
	public:

		NamedPattern(Token name, Token pattern)
			: name(std::move(name)), pattern(std::move(pattern))
		{ }

		~NamedPattern() override = default;


		void acceptVisitor(NodeVisitor& visitor) const override;


		Token name;
		Token pattern;
	};

}

#endif