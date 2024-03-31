#ifndef PARSEC_PARS_AST_BINARY_RULE_HEADER
#define PARSEC_PARS_AST_BINARY_RULE_HEADER

#include "Node.hpp"

namespace parsec::pars::ast {

	/**
	 * @brief Rule consisting of two subrules.
	*/
	class BinaryRule : public Node {
	public:

		BinaryRule(NodePtr left, NodePtr right)
			: m_left(std::move(left)), m_right(std::move(right)) {}

		~BinaryRule() override = default;
		

		/**
		 * @brief Left subrule.
		*/
		const Node* left() const {
			return m_left.get();
		}


		/**
		 * @brief Right subrule.
		*/
		const Node* right() const {
			return m_right.get();
		}


	private:
		NodePtr m_left;
		NodePtr m_right;
	};

}

#endif