#pragma once

#include <memory>

namespace parsec::regex {

    class NodeVisitor;

    class ExprNode {
    public:

        ExprNode() = default;

        ExprNode(const ExprNode& other) = delete;
        ExprNode& operator=(const ExprNode& other) = delete;

        ExprNode(ExprNode&& other) = delete;
        ExprNode& operator=(ExprNode&& other) = delete;

        virtual ~ExprNode() = default;


        virtual void accept(NodeVisitor& visitor) const = 0;

        virtual bool isNullable() const noexcept = 0;

        virtual int atomCount() const noexcept = 0;
    };

    using NodePtr = std::shared_ptr<ExprNode>;

}
