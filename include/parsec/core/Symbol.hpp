#pragma once

#include "../util/string_util.hpp"

#include <compare>
#include <cstddef>
#include <functional>
#include <memory>
#include <ostream>
#include <string>
#include <string_view>

namespace parsec {

    /**
     * @brief Immutable symbolic name.
     */
    class Symbol {
    public:

        friend bool operator==(const Symbol& lhs, const Symbol& rhs) noexcept {
            return lhs.value() == rhs.value();
        }

        friend std::strong_ordering operator<=>(const Symbol& lhs, const Symbol& rhs) noexcept {
            return lhs.value() <=> rhs.value();
        }


        Symbol(const Symbol& other) noexcept = default;
        Symbol& operator=(const Symbol& other) noexcept = default;

        Symbol(Symbol&& other) noexcept = default;
        Symbol& operator=(Symbol&& other) noexcept = default;

        ~Symbol() noexcept = default;


        /** @{ */
        /**
         * @brief Construct an empty symbol.
         */
        Symbol() noexcept = default;


        /**
         * @brief Construct a symbol from a single character.
         */
        Symbol(char value)
            : Symbol(std::string_view(&value, 1)) {}


        /**
         * @brief Construct a symbol from a string literal.
         */
        Symbol(const char* value)
            : Symbol(std::string_view(value)) {}


        /**
         * @brief Construct a symbol from a string.
         */
        Symbol(const std::string& value)
            : Symbol(std::string_view(value)) {}


        /**
         * @brief Construct a symbol from an arbitrary character sequence.
         */
        Symbol(std::string_view value)
            : value_(value.empty() ? nullptr : std::make_shared<std::string>(string_util::escape(value))) {}
        /** @} */


        /** @{ */
        /**
         * @brief A character string representing the symbol's name.
         */
        const std::string& value() const noexcept {
            if(isEmpty()) {
                static const std::string empty;
                return empty;
            }
            return *value_;
        }


        /**
         * @brief Check if the symbol has a non-empty value.
         */
        explicit operator bool() const noexcept {
            return !isEmpty();
        }


        /**
         * @brief Check if the symbol has an empty value.
         */
        bool isEmpty() const noexcept {
            return !value_;
        }
        /** @} */


    private:
        std::shared_ptr<std::string> value_;
    };

    std::ostream& operator<<(std::ostream& out, const Symbol& symbol);

}

template <>
struct std::hash<parsec::Symbol> {
    std::size_t operator()(const parsec::Symbol& symbol) const noexcept {
        return std::hash<std::string>()(symbol.value());
    }
};
