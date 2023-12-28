#ifndef PARSEC_PARS_AST_INLINE_PATTERN_HEADER
#define PARSEC_PARS_AST_INLINE_PATTERN_HEADER

#include "../Token.hpp"
#include "Node.hpp"

namespace parsec::pars::ast {

	/**
	 * @brief Represents a string pattern inlined into a rule.
	*/
	class InlinePattern : public Node {
	public:

		explicit InlinePattern(Token pattern)
			: pattern(std::move(pattern))
		{ }

		~InlinePattern() override = default;


		void acceptVisitor(NodeVisitor& visitor) const override;


		Token pattern;
	};

}

#endif