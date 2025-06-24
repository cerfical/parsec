#pragma once

#include "SourceLoc.hpp"

#include <stdexcept>

namespace parsec {

    /**
     * @brief Describes an unexpected end-of-file error.
     */
    class UnexpectedEofError : public std::runtime_error {
    public:

        /**
         * @brief Construct a new error.
         *
         * @param loc The location where the error occurred.
         */
        explicit UnexpectedEofError(const SourceLoc& loc)
            : std::runtime_error("unexpected end of file")
            , loc_(loc) {}


        /**
         * @brief The error location.
         */
        const SourceLoc& loc() const noexcept {
            return loc_;
        }


    private:
        SourceLoc loc_;
    };

}
