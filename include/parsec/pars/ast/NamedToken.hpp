#ifndef PARSEC_PARS_AST_NAMED_TOKEN_HEADER
#define PARSEC_PARS_AST_NAMED_TOKEN_HEADER

#include "../Token.hpp"
#include "Node.hpp"

namespace parsec::pars {

	/**
	 * @brief Definition of a named token.
	*/
	class NamedToken : public Node {
	public:

		NamedToken(const Token& name, const Token& pattern)
			: m_name(name), m_pattern(pattern) {}

		~NamedToken() override = default;


		void accept(NodeVisitor& visitor) const override;


		/**
		 * @brief Name of the token.
		*/
		const Token& name() const {
			return m_name;
		}


		/**
		 * @brief String pattern defining the token.
		*/
		const Token& pattern() const {
			return m_pattern;
		}


	private:
		Token m_name;
		Token m_pattern;
	};

}

#endif