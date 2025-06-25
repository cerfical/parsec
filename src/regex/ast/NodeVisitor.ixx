export module parsec.regex:ast.NodeVisitor;

export import :ast.AtomExprNode;

export import :ast.StarClosureNode;
export import :ast.PlusClosureNode;
export import :ast.OptionalExprNode;

export import :ast.AlternExprNode;
export import :ast.ConcatExprNode;

namespace parsec::regex {

    /**
     * @brief Implements a visitor for ExprNode%s.
     */
    export class NodeVisitor {
    public:

        /**
         * @brief Called for a AtomExprNode.
         */
        virtual void visit(const AtomExprNode& n) = 0;


        /**
         * @brief Called for a StarClosureNode.
         */
        virtual void visit(const StarClosureNode& n) = 0;


        /**
         * @brief Called for a PlusClosureNode.
         */
        virtual void visit(const PlusClosureNode& n) = 0;


        /**
         * @brief Called for an OptionalExprNode.
         */
        virtual void visit(const OptionalExprNode& n) = 0;


        /**
         * @brief Called for an AlternExprNode.
         */
        virtual void visit(const AlternExprNode& n) = 0;


        /**
         * @brief Called for a ConcatExprNode.
         */
        virtual void visit(const ConcatExprNode& n) = 0;


    protected:
        ~NodeVisitor() = default;
    };

}
