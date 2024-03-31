#ifndef PARSEC_PARS_AST_INLINE_TOKEN_HEADER
#define PARSEC_PARS_AST_INLINE_TOKEN_HEADER

#include "../Token.hpp"
#include "Node.hpp"

namespace parsec::pars::ast {

	/**
	 * @brief Definition of an inline token without a name.
	*/
	class InlineToken : public Node {
	public:

		explicit InlineToken(const Token& pattern)
			: m_pattern(pattern) {}

		~InlineToken() override = default;


		void acceptVisitor(NodeVisitor& visitor) const override;


		/**
		 * @brief String pattern for the token.
		*/
		const Token& pattern() const {
			return m_pattern;
		}


	private:
		Token m_pattern;
	};

}

#endif