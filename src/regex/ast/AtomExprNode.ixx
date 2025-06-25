module;

#include <string>
#include <utility>

export module parsec.regex:ast.AtomExprNode;

import :ast.ExprNode;

namespace parsec::regex {

    /**
     * @brief A basic expression representing a single atomic value.
     */
    export class AtomExprNode : public ExprNode {
    public:

        explicit AtomExprNode(std::string value)
            : value_(std::move(value)) {}


        void accept(NodeVisitor& visitor) const override;

        bool isNullable() const noexcept override {
            return isNull();
        }

        int atomCount() const noexcept override {
            return !isNull() ? 1 : 0;
        }


        /**
         * @brief Value of the symbol.
         */
        const std::string& value() const noexcept {
            return value_;
        }

        bool isNull() const noexcept {
            return value_.empty();
        }


    private:
        std::string value_;
    };

}
