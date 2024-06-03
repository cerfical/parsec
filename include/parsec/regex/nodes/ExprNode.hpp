#pragma once

#include "../util/util_types.hpp"

#include <memory>
#include <ostream>

namespace parsec::regex {

	class NodeVisitor;

	class ExprNode : private NonCopyable, private NonMovable {
	public:

		virtual ~ExprNode() = default;


		virtual void accept(NodeVisitor& visitor) const = 0;

		virtual bool isNullable() const noexcept = 0;

		virtual int atomCount() const noexcept = 0;

	};

	using NodePtr = std::shared_ptr<ExprNode>;

}
