module;

#include <memory>
#include <ostream>

export module parsec.regex:ast.ExprNode;

namespace parsec::regex {

    export class NodeVisitor;

    /**
     * @brief AST node to represent a regular expression.
     */
    export class ExprNode {
    public:

        ExprNode() = default;

        ExprNode(const ExprNode& other) = delete;
        ExprNode& operator=(const ExprNode& other) = delete;

        ExprNode(ExprNode&& other) = delete;
        ExprNode& operator=(ExprNode&& other) = delete;

        virtual ~ExprNode() = default;


        /** @{ */
        /**
         * @brief Perform a type-safe 'visit' on the node using a visitor.
         */
        virtual void accept(NodeVisitor& visitor) const = 0;


        /**
         * @brief Check if the expression can generate an empty string.
         */
        virtual bool isNullable() const noexcept = 0;


        /**
         * @brief Count the number of AtomExprNode%s in the expression.
         */
        virtual int atomCount() const noexcept = 0;
        /** @} */
    };

    export std::ostream& operator<<(std::ostream& out, const ExprNode& n);


    /**
     * @brief Owning pointer to an ExprNode.
     */
    export using NodePtr = std::shared_ptr<ExprNode>;

}
