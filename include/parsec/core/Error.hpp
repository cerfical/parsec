#pragma once

#include <stdexcept>
#include <string>

namespace parsec {

    /**
     * @brief Represents runtime errors generated by the library.
     */
    class Error : public std::runtime_error {
    public:

        /**
         * @brief Construct a generic error with a short description of the problem.
         *
         * @param msg The description of the problem.
         */
        explicit Error(const std::string& msg)
            : runtime_error(msg) {}
    };

}
