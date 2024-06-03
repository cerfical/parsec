#pragma once

#include "../Token.hpp"
#include "Node.hpp"

namespace parsec::pars {

	/**
	 * @brief Represents a rule with a name.
	*/
	class NamedRule : public Node {
	public:
			
		NamedRule(const Token& name, NodePtr rule)
			: m_name(name), m_rule(std::move(rule)) {}

		~NamedRule() override = default;


		void accept(NodeVisitor& visitor) const override;


		/**
		 * @brief Name of the rule.
		*/
		const Token& name() const {
			return m_name;
		}


		/**
		 * @brief Body of the rule.
		*/
		const Node* rule() const {
			return m_rule.get();
		}


	private:
		Token m_name;
		NodePtr m_rule;
	};

}
