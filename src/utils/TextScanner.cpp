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


	char TextScanner::get() {
		if(!eof()) {
			const auto ch = gsl::narrow_cast<char>(m_input->get());
			if(m_pos++; ch == '\n') {
				m_linePos = m_pos;
				m_lineNo++;
			}
			return ch;
		}
		unexpectedEof();
	}

	char TextScanner::peek() const {
		if(!eof()) {
			return gsl::narrow_cast<char>(m_input->peek());
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
}