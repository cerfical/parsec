#include "regex.hpp"

#include "core/TextScanner.hpp"
#include "utils/char_utils.hpp"

#include <sstream>
#include <format>

namespace parsec::regex {
	namespace {
		class ParseInputStream {
		public:

			explicit ParseInputStream(std::istream& input) noexcept
				: m_scanner(input) {}
			
			RegularExpr operator()() {
				// no input, nothing to parse
				if(m_scanner.isEof()) {
					return RegularExpr();
				}

				auto e = parseExpr();
				if(!m_scanner.isEof()) {
					unexpected();
				}
				return e;
			}

		private:
			[[noreturn]]
			void unexpected() const {
				if(m_scanner.peek() != ')') {
					throw ParseError(std::format("unexpected '{}'",
						char_utils::escape(m_scanner.peek())
						), m_scanner.pos()
					);
				}
				unmatchedParen(m_scanner.pos());
			}
			
			[[noreturn]]
			void unmatchedParen(gsl::index parenLoc) const {
				throw ParseError("unmatched parenthesis", parenLoc);
			}

			[[noreturn]]
			void invalidHexSeq() const {
				throw ParseError("expected at least one hexadecimal digit",
					m_scanner.pos()
				);
			}

			[[noreturn]]
			void outOfOrderCharRange(gsl::index charRangeStart) const {
				throw ParseError("character range is out of order",
					IndexRange(charRangeStart, m_scanner.pos())
				);
			}


			bool isAtom() const {
				if(!m_scanner.isEof()) {
					switch(m_scanner.peek()) {
						case '*': case '|': case ')': case ']': case '?': case '+': break;
						case '(': case '[': case '\\': default: return true;
					}
				}
				return false;
			}

			char parseEscapeSeq() {
				switch(m_scanner.peek()) {
					case '0': { m_scanner.skip(); return '\0'; }
					case 'a': { m_scanner.skip(); return '\a'; }
					case 'b': { m_scanner.skip(); return '\b'; }
					case 'f': { m_scanner.skip(); return '\f'; }
					case 'n': { m_scanner.skip(); return '\n'; }
					case 'r': { m_scanner.skip(); return '\r'; }
					case 't': { m_scanner.skip(); return '\t'; }
					case 'v': { m_scanner.skip(); return '\v'; }
					case 'x': {
						m_scanner.skip();
						if(char_utils::isHexDigit(m_scanner.peek())) {
							auto ch = char_utils::evalHexDigit(m_scanner.get());
							if(!m_scanner.isEof() && char_utils::isHexDigit(m_scanner.peek())) {
								ch = ch * 16 + char_utils::evalHexDigit(m_scanner.get());
							}
							return gsl::narrow_cast<char>(ch);
						}
						invalidHexSeq();
					}
				}
				return m_scanner.get();
			}
			
			char parseChar() {
				if(m_scanner.skipIf('\\')) {
					return parseEscapeSeq();
				}
				return m_scanner.get();
			}


			RegularExpr parseCharRange() {
				// save the position and value of the lower bound of the possible character range
				const auto charRangeStart = m_scanner.pos();
				const auto low = parseChar();

				// no char range, just a single character
				auto e = RegularExpr(Symbol(low));
				if(!m_scanner.skipIf('-')) {
					return e;
				}

				// string of the form 'l-h' is a character range
				if(m_scanner.peek() != ']') {
					const auto high = parseChar();
					if(low > high) {
						outOfOrderCharRange(charRangeStart);
					}

					for(auto ch = low + 1; ch <= high; ) {
						e |= Symbol(ch++);
					}
				} else {
					e |= Symbol('-');
				}

				return e;
			}
			
			RegularExpr parseCharSet() {
				// empty character set
				if(m_scanner.skipIf(']')) {
					return Symbol();
				}

				auto lhs = parseCharRange();
				while(m_scanner.peek() != ']') {
					lhs |= parseCharRange();
				}
				m_scanner.skip(); // skip ']'
				return lhs;
			}


			RegularExpr parseAtom() {
				if(!isAtom()) {
					unexpected();
				}

				if(const auto openParen = m_scanner.pos(); m_scanner.skipIf('(')) {
					// empty parenthesized expression
					if(m_scanner.skipIf(')')) {
						return Symbol();
					}

					auto e = parseExpr();
					if(!m_scanner.skipIf(')')) {
						unmatchedParen(openParen);
					}
					return e;
				}

				if(m_scanner.skipIf('[')) {
					return parseCharSet();
				}

				return RegularExpr(parseChar());
			}

			RegularExpr parseRepeat() {
				auto e = parseAtom();
				while(true) {
					if(m_scanner.skipIf('*')) {
						e = starClosure(e);
					} else if(m_scanner.skipIf('?')) {
						e = optional(e);
					} else if(m_scanner.skipIf('+')) {
						e = plusClosure(e);
					} else {
						break;
					}
				}
				return e;
			}

			RegularExpr parseConcat() {
				auto lhs = parseRepeat();
				while(isAtom()) {
					lhs += parseRepeat();
				}
				return lhs;
			}

			RegularExpr parseAltern() {
				auto lhs = parseConcat();
				while(m_scanner.skipIf('|')) {
					lhs |= parseConcat();
				}
				return lhs;
			}
			
			RegularExpr parseExpr() {
				return parseAltern();
			}


			TextScanner m_scanner;
		};
	}


	RegularExpr Parser::parse(std::string_view regex) {
		auto input = std::istringstream(std::string(regex));
		return parse(input);
	}

	RegularExpr Parser::parse(std::istream& input) {
		return ParseInputStream(input)();
	}
}