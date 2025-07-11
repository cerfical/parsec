module;

#include <cstddef>
#include <istream>
#include <string>
#include <string_view>

export module parsec.scan:TextScanner;

import :SourceLoc;

namespace parsec::scan {

    /**
     * @brief Facilitates textual data analysis using standard stream classes.
     */
    export class TextScanner {
    public:

        TextScanner() = default;

        TextScanner(const TextScanner&) = delete;
        TextScanner& operator=(const TextScanner&) = delete;

        TextScanner(TextScanner&&) noexcept = default;
        TextScanner& operator=(TextScanner&&) noexcept = default;

        ~TextScanner() = default;


        /** @{ */
        /**
         * @brief Construct a new scanner to operate on an input stream.
         */
        explicit TextScanner(std::istream* input)
            : input_(input) {}


        /**
         * @brief Read and remove from the stream the next input character.
         */
        char get();


        /**
         * @brief Read a character at the offset from the current position without removing it.
         */
        char peek(int off = 0) const;


        /**
         * @brief Remove a character from the input if it matches specified character.
         * @returns @c true if a skip has taken place, @c false otherwise.
         */
        bool skipIf(char ch);


        /**
         * @brief Remove a character sequence from the input if it matches specified text.
         * @returns @c true if a skip has taken place, @c false otherwise.
         */
        bool skipIf(std::string_view text);


        /**
         * @brief Remove the next character from the input.
         */
        void skip() {
            get();
        }


        /**
         * @brief Check whether the end of input has been reached.
         */
        bool isEof() const {
            return labuf_.empty() && checkForEof();
        }


        /**
         * @brief The current position of the scanner in the input stream.
         */
        const SourceLoc& pos() const {
            return pos_;
        }
        /** @} */


    private:
        bool fillLookaheadBuffer(std::size_t size) const;

        void updateLocInfo(char ch) noexcept;
        bool checkForEof() const;


        mutable std::string labuf_;
        std::istream* input_ = {};

        SourceLoc pos_ = {
            .offset = 0,
            .colCount = 1,
        };
    };

}
