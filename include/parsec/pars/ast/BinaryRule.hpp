#pragma once

#include "Node.hpp"

namespace parsec::pars {

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
