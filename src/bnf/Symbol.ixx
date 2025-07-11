module;

#include <compare>
#include <cstddef>
#include <functional>
#include <memory>
#include <string>
#include <string_view>

export module parsec.bnf:Symbol;

namespace parsec::bnf {

    /**
     * @brief Immutable symbolic name.
     */
    export class Symbol {
    public:

        friend bool operator==(const Symbol& lhs, const Symbol& rhs) noexcept {
            return lhs.text() == rhs.text();
        }

        friend std::strong_ordering operator<=>(const Symbol& lhs, const Symbol& rhs) noexcept {
            return lhs.text() <=> rhs.text();
        }


        /**
         * @brief Construct a symbol from a single character.
         */
        Symbol(char ch)
            : Symbol(std::string_view(&ch, 1)) {}


        /**
         * @brief Construct a symbol from a string literal.
         */
        Symbol(const char* cstr)
            : Symbol(std::string_view(cstr)) {}


        /**
         * @brief Construct a symbol from a string.
         */
        Symbol(const std::string& str)
            : Symbol(std::string_view(str)) {}


        /**
         * @brief Construct a symbol from an arbitrary character sequence.
         */
        Symbol(std::string_view str = "")
            : text_(std::make_shared<std::string>(str)) {}


        /**
         * @brief A character string representing the symbol's name.
         */
        const std::string& text() const noexcept {
            return *text_;
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
            return text_->empty();
        }


    private:
        std::shared_ptr<std::string> text_;
    };

}

template <>
struct std::hash<parsec::bnf::Symbol> {
    std::size_t operator()(const parsec::bnf::Symbol& symbol) const noexcept {
        return std::hash<std::string>()(symbol.text());
    }
};
