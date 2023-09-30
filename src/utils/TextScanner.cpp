#include "utils/TextScanner.hpp"
#include "utils/ParseError.hpp"

namespace parsec {
	void TextScanner::badEof() const {
		// construct a location that points to the offending symbol
		const auto loc = SourceLoc(m_pos - m_linePos, 1, m_lineNo, m_linePos);
		throw ParseError("unexpected end of input", loc);
	}


	bool TextScanner::eof() const {
		if(m_input) {
			// if the of end of file was reached, reset the eof bit
			if(m_input->peek() != std::char_traits<char>::eof()) {
				return false;
			}
			m_input->clear(m_input->rdstate() ^ std::ios_base::eofbit);
		}
		return true;
	}

	char TextScanner::peek() const {
		if(!eof()) {
			return gsl::narrow_cast<char>(m_input->peek());
		}
		badEof();
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

	char TextScanner::get() {
		if(!eof()) {
			const auto ch = gsl::narrow_cast<char>(m_input->get());
			if(m_pos++; ch == '\n') {
				m_linePos = m_pos;
				m_lineNo++;
			}
			return ch;
		}
		badEof();
	}
}