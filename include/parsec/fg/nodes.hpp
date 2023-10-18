#ifndef PARSEC_FG_NODES_HEADER
#define PARSEC_FG_NODES_HEADER

#include "GrammarNode.hpp"

#include <string>
#include <memory>

namespace parsec::fg {
	/**
	 * @brief Reference to a grammar rule defined elsewhere.
	 */
	class RuleRef : public GrammarNode {
	public:
		/** @{ */
		/** @brief Define a new reference to a grammar rule. */
		explicit RuleRef(const std::string& ruleName)
			: m_ruleName(ruleName)
		{ }
		

		/** @brief Destroy the node. */
		~RuleRef() override = default;
		/** @} */


		/** @{ */
		/** @brief Name of the referenced rule. */
		const std::string& ruleName() const noexcept {
			return m_ruleName;
		}
		/** @} */


		/** @{ */
		void print(std::ostream& out) const override {
			out << m_ruleName;
		}
		/** @} */


	private:
		std::string m_ruleName;
	};



	/**
	 * @brief Binary grammar node.
	 */
	class BinaryNode : public GrammarNode {
	public:
		/** @{ */
		/** @brief Construct a new node node with two child nodes. */
		BinaryNode(
			std::unique_ptr<GrammarNode> left,
			std::unique_ptr<GrammarNode> right
		) noexcept
			: m_left(std::move(left)), m_right(std::move(right))
		{ }
		/** @} */


		/** @{ */
		/** @brief Left child of the node. */
		const GrammarNode* left() const noexcept {
			return m_left.get();
		}


		/** @brief Right child of the node. */
		const GrammarNode* right() const noexcept {
			return m_right.get();
		}
		/** @} */


	private:
		std::unique_ptr<GrammarNode> m_left;
		std::unique_ptr<GrammarNode> m_right;
	};



	/**
	 * @brief Alternation node.
	 */
	class AlternNode : public BinaryNode {
	public:
		/** @{ */
		/** @brief Construct a new node node with two child nodes. */
		AlternNode(
			std::unique_ptr<GrammarNode> left,
			std::unique_ptr<GrammarNode> right
		) noexcept
			: BinaryNode(std::move(left), std::move(right))
		{ }
		/** @} */


		/** @{ */
		void print(std::ostream& out) const override {
			out << '(';
			left()->print(out);
			out << " | ";
			right()->print(out);
			out << ')';
		}
		/** @} */
	};



	/**
	 * @brief Concatenation node.
	 */
	class ConcatNode : public BinaryNode {
	public:
		/** @{ */
		/** @brief Construct a new node node with two child nodes. */
		ConcatNode(
			std::unique_ptr<GrammarNode> left,
			std::unique_ptr<GrammarNode> right
		) noexcept
			: BinaryNode(std::move(left), std::move(right))
		{ }
		/** @} */


		/** @{ */
		void print(std::ostream& out) const override {
			out << '(';
			left()->print(out);
			out << ' ';
			right()->print(out);
			out << ')';
		}
		/** @} */
	};
}

#endif