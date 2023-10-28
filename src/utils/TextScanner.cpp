#include "utils/TextScanner.hpp"
#include "utils/ParseError.hpp"

#include <gsl/narrow>

namespace parsec {
	void TextScanner::unexpectedEof() const {
		throw ParseError("unexpected end of input",
			SourceLoc(
				m_pos - m_linePos, // column number
				1, // column count
				m_lineNo,
				m_linePos
			)
		);
	}


	bool TextScanner::safeEof() const {
		if(m_input->peek() == std::char_traits<char>::eof()) {
			m_input->clear(m_input->rdstate() ^ std::ios::eofbit);
			return true;
		}
		return false;
	}
	
	bool TextScanner::fillBuf(int size) const {
		// fill the lookahead buffer with the required number of characters
		while(m_labuf.size() < size) {
			if(safeEof()) {
				return false;
			}
			m_labuf += m_input->get();
		}
		return true;
	}


	char TextScanner::get() {
		// take the character from the buffer, if there is any
		if(!m_labuf.empty()) {
			const auto ch = m_labuf.front();
			m_labuf.erase(0, 1);
			return ch;
		}

		// fallback to calling iostreams api
		if(!safeEof()) {
			const auto ch = gsl::narrow_cast<char>(m_input->get());
			m_pos++;

			if(ch == '\n') {
				m_linePos = m_pos;
				m_lineNo++;
			}
			return ch;
		}
		unexpectedEof();
	}

	char TextScanner::peek(int i) const {
		if(fillBuf(i + 1)) {
			return m_labuf[i];
		}
		unexpectedEof();
	}


	bool TextScanner::skipIf(char ch) {
		if(!eof()) {
			if(peek() == ch) {
				skip();
				return true;
			}
		}
		return false;
	}

	bool TextScanner::skipIf(std::string_view text) {
		if(!fillBuf(text.size())) {
			return false;
		}

		// compare the text for equality with a same-sized substring of the lookahead buffer
		if(m_labuf.compare(0, text.size(), text) == 0) {
			m_labuf.erase(0, text.size());
			m_pos += text.size();
			return true;
		}
		return false;
	}
}