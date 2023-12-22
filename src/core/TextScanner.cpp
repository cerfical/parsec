#include "core/TextScanner.hpp"
#include "core/EofError.hpp"

#include <gsl/narrow>

namespace parsec {
	void TextScanner::unexpectedEof() const {
		throw EofError(pos());
	}


	bool TextScanner::checkForEof() const {
		if(m_input->peek() == std::char_traits<char>::eof()) {
			m_input->clear(m_input->rdstate() ^ std::ios::eofbit);
			return true;
		}
		return false;
	}
	
	void TextScanner::updateLoc(char ch) {
		m_pos++;
		if(ch == '\n') {
			m_linePos = m_pos;
			m_lineNo++;
		}
	}


	bool TextScanner::fillBuf(int size) const {
		// fill the lookahead buffer with the required number of characters
		while(m_labuf.size() < size) {
			if(checkForEof()) {
				return false;
			}
			m_labuf += m_input->get();
		}
		return true;
	}


	char TextScanner::get() {
		char ch = '\0';
		// first check the buffer to take the character from
		if(!m_labuf.empty()) {
			ch = m_labuf.front();
			m_labuf.erase(0, 1);
		} else if(!checkForEof()) {
			// otherwise fallback to calling iostreams api
			ch = gsl::narrow_cast<char>(m_input->get());
		} else {
			unexpectedEof();
		}

		// finally update location information, based on the consumed character
		updateLoc(ch);
		return ch;
	}

	char TextScanner::peek(int i) const {
		if(fillBuf(i + 1)) {
			return m_labuf[i];
		}
		unexpectedEof();
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
		if(!fillBuf(text.size())) {
			return false;
		}

		// compare the text for equality with a same-sized substring of the lookahead buffer
		if(m_labuf.compare(0, text.size(), text) == 0) {
			m_labuf.erase(0, text.size());

			for(const auto ch : text) {
				updateLoc(ch);
			}
			return true;
		}
		return false;
	}
}