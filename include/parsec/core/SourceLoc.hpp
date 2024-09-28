#pragma once

#include <ostream>

namespace parsec {

    /**
     * @brief Describes a line inside some text.
     */
    struct LineInfo {

        /**
         * @brief The absolute position of the line in the text.
         */
        int offset = {};


        /**
         * @brief The ordinal number of the line.
         */
        int no = {};
    };


    /**
     * @brief Describes a location inside a single line of text.
     */
    struct SourceLoc {

        /**
         * @brief Check whether the location is not empty, i.e., contains at least one column.
         */
        explicit operator bool() const noexcept {
            return !isEmpty();
        }


        /**
         * @brief Check whether the location is empty, i.e., contains no columns.
         */
        bool isEmpty() const noexcept {
            return colCount == 0;
        }


        /**
         * @brief The relative position of the first column.
         */
        int startCol() const noexcept {
            return offset - line.offset;
        }


        /**
         * @brief The relative position of the after-the-last column.
         */
        int endCol() const noexcept {
            return startCol() + colCount;
        }


        /**
         * @brief The absolute position of the location in the text.
         */
        int offset = {};


        /**
         * @brief The number of characters spanning the location.
         */
        int colCount = {};


        /**
         * @brief The line containing the location.
         */
        LineInfo line;
    };

    std::ostream& operator<<(std::ostream& out, const SourceLoc& loc);

}
