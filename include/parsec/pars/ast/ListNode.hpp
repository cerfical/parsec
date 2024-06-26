#pragma once

#include "Node.hpp"

namespace parsec::pars {

	/**
	 * @brief Sequence of two other nodes.
	*/
	class ListNode : public Node {
	public:

		ListNode(NodePtr head, NodePtr tail)
			: m_head(std::move(head)), m_tail(std::move(tail)) {}

		~ListNode() override = default;


		void accept(NodeVisitor& visitor) const override;


		/**
		 * @brief Head node of the list.
		*/
		const Node* head() const {
			return m_head.get();
		}


		/**
		 * @brief Tail node of the list.
		*/
		const Node* tail() const {
			return m_tail.get();
		}


	private:
		NodePtr m_head;
		NodePtr m_tail;
	};

}
