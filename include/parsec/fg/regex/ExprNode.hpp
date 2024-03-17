#ifndef PARSEC_FG_REGEX_EXPR_NODE_HEADER
#define PARSEC_FG_REGEX_EXPR_NODE_HEADER

#include "../../core/NonCopyable.hpp"
#include "../../core/NonMovable.hpp"

#include <memory>
#include <ostream>

namespace parsec::fg::regex {

	class NodeVisitor;

	class ExprNode : private NonCopyable, private NonMovable {
	public:

		virtual ~ExprNode() = default;


		virtual void accept(NodeVisitor& visitor) const = 0;

		virtual bool isNullable() const noexcept = 0;

		
		void printTo(std::ostream& out) const;

	};

	using NodePtr = std::shared_ptr<ExprNode>;



	template <typename Node, typename... Args>
		requires std::derived_from<Node, ExprNode>
	NodePtr makeNode(Args&&... args) {
		return std::make_shared<Node>(std::forward<Args>(args)...);
	}

}

#endif