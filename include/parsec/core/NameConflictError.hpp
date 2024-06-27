#pragma once

#include "Error.hpp"
#include "Symbol.hpp"

#include <utility>

namespace parsec {

    /**
     * @brief Indicates that there exists some conflict between several symbolic names.
     */
    class NameConflictError : public Error {
    public:

        /**
         * @brief Construct an error from two conflicting names.
         */
        NameConflictError(Symbol name1, Symbol name2)
            : Error("name conflict error"), name1_(std::move(name1)), name2_(std::move(name2)) {}


        /**
         * @brief The first conflicting name.
         */
        const Symbol& name1() const noexcept {
            return name1_;
        }


        /**
         * @brief The second conflicting name.
         */
        const Symbol& name2() const noexcept {
            return name2_;
        }


    private:
        Symbol name1_;
        Symbol name2_;
    };

}
