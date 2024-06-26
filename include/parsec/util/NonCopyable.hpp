#pragma once

namespace parsec {

    /**
     * @brief Mixin type to prevent copying objects of another type.
     */
    struct NonCopyable {

        NonCopyable() = default;
        ~NonCopyable() = default;

        NonCopyable(const NonCopyable&) = delete;
        NonCopyable& operator=(const NonCopyable&) = delete;

        NonCopyable(NonCopyable&&) = default;
        NonCopyable& operator=(NonCopyable&&) = default;
    };

}
