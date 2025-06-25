module;

#include <ostream>

export module parsec.scan:SourceLoc;

namespace parsec::scan {

    /**
     * @brief Describes a line inside some text.
     */
    export struct LineInfo {

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
    export struct SourceLoc {

        /**
         * @brief Check whether the location encompasses any data.
         */
        explicit operator bool() const noexcept {
            return !isEof();
        }


        /**
         * @brief Check whether the location marks the end of file.
         */
        bool isEof() const noexcept {
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


    export std::ostream& operator<<(std::ostream& out, const SourceLoc& loc) {
        out << loc.line.no + 1 << ':' << loc.startCol() + 1;
        if(loc) {
            out << '-' << (loc.endCol() - 1) + 1;
        }
        return out;
    }

}
