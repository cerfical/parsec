module;

#include "bnf/Symbol.hpp"

#include <stdexcept>
#include <utility>

export module parsec.fsm:NameConflictError;

namespace parsec::fsm {

    /**
     * @brief Indicates that there exists some conflict between several symbolic names.
     */
    export class NameConflictError : public std::runtime_error {
    public:

        /**
         * @brief Construct an error from two conflicting names.
         */
        NameConflictError(bnf::Symbol name1, bnf::Symbol name2)
            : std::runtime_error("name conflict error")
            , name1_(std::move(name1))
            , name2_(std::move(name2)) {}


        /**
         * @brief The first conflicting name.
         */
        const bnf::Symbol& name1() const noexcept {
            return name1_;
        }


        /**
         * @brief The second conflicting name.
         */
        const bnf::Symbol& name2() const noexcept {
            return name2_;
        }


    private:
        bnf::Symbol name1_;
        bnf::Symbol name2_;
    };

}
