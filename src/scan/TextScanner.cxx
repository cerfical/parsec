module;

#include <cstddef>
#include <ios>
#include <string>
#include <string_view>

module parsec.scan;

namespace parsec::scan {
    char TextScanner::get() {
        char ch = {};
        // first check the buffer to take the character from
        if(!labuf_.empty()) {
            ch = labuf_.front();
            labuf_.erase(0, 1);
        } else if(!checkForEof()) {
            // otherwise fallback to calling istream api
            ch = static_cast<char>(input_->get());
        } else {
            throw UnexpectedEofError(pos());
        }

        // finally update location information, based on the consumed character
        updateLocInfo(ch);
        return ch;
    }


    char TextScanner::peek(int off) const {
        if(fillLookaheadBuffer(off + 1)) {
            return labuf_[off];
        }
        throw UnexpectedEofError(pos());
    }


    bool TextScanner::skipIf(char ch) {
        if(!isEof()) {
            if(peek() == ch) {
                skip();
                return true;
            }
        }
        return false;
    }


    bool TextScanner::skipIf(std::string_view text) {
        if(!fillLookaheadBuffer(text.size())) {
            return false;
        }

        // compare the text for equality with a same-sized substring of the lookahead buffer
        if(labuf_.starts_with(text)) {
            labuf_.erase(0, text.size());
            for(char ch : text) {
                updateLocInfo(ch);
            }
            return true;
        }
        return false;
    }


    bool TextScanner::fillLookaheadBuffer(std::size_t size) const {
        // fill the lookahead buffer with the required number of characters
        while(labuf_.size() < size) {
            if(checkForEof()) {
                return false;
            }
            labuf_ += static_cast<char>(input_->get());
        }
        return true;
    }

    bool TextScanner::checkForEof() const {
        if(!input_ || input_->eof() || input_->fail() || input_->bad()) {
            return true;
        }

        if(input_->peek() == std::char_traits<char>::eof()) {
            input_->clear(input_->rdstate() ^ std::ios::eofbit);
            return true;
        }
        return false;
    }

    void TextScanner::updateLocInfo(char ch) noexcept {
        pos_.offset++;
        if(ch == '\n') {
            pos_.line.offset = pos_.offset;
            pos_.line.no++;
        }
    }
}
