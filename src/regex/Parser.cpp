#include "regex/Parser.hpp"
#include "regex/make.hpp"

#include "core/TextScanner.hpp"
#include "core/ParseError.hpp"

#include "utils/char_utils.hpp"

#include <sstream>

namespace parsec::regex {
	namespace {
		class ParserImpl {
		public:

			explicit ParserImpl(std::istream& input)
				: m_input(input) {}
			
			NodePtr run() {
				// no input, nothing to parse
				if(m_input.isEof()) {
					return empty();
				}

				auto e = parseExpr();
				if(!m_input.isEof()) {
					throw ParseError::misplacedChar(m_input.loc(), m_input.peek());
				}
				return e;
			}

		private:

			NodePtr parseExpr() {
				return parseAltern();
			}


			NodePtr parseAltern() {
				auto lhs = parseConcat();
				while(m_input.skipIf('|')) {
					lhs = altern(lhs, parseConcat());
				}
				return lhs;
			}


			NodePtr parseConcat() {
				auto lhs = parseRepeat();
				while(isAtom()) {
					lhs = concat(lhs, parseRepeat());
				}
				return lhs;
			}
			
			
			NodePtr parseRepeat() {
				auto e = parseAtom();
				while(true) {
					if(m_input.skipIf('*')) {
						e = starClosure(e);
					} else if(m_input.skipIf('?')) {
						e = optional(e);
					} else if(m_input.skipIf('+')) {
						e = plusClosure(e);
					} else {
						break;
					}
				}
				return e;
			}


			NodePtr parseAtom() {
				if(!isAtom()) {
					throw ParseError::misplacedChar(m_input.loc(), m_input.peek());
				}

				if(const auto openParen = m_input.loc(); m_input.skipIf('(')) {
					// empty parenthesized expression
					if(m_input.skipIf(')')) {
						return empty();
					}

					auto e = parseExpr();
					if(!m_input.skipIf(')')) {
						throw ParseError::misplacedChar(openParen, '(');
					}
					return e;
				}

				if(m_input.skipIf('[')) {
					return parseCharSet();
				}

				return atom(parseChar());
			}


			bool isAtom() const {
				if(!m_input.isEof()) {
					switch(m_input.peek()) {
						case '*': case '|': case ')': case ']': case '?': case '+': break;
						case '(': case '[': case '\\': default: return true;
					}
				}
				return false;
			}


			NodePtr parseCharSet() {
				// empty character set
				if(m_input.skipIf(']')) {
					return empty();
				}

				auto lhs = parseCharRange();
				while(m_input.peek() != ']') {
					lhs = altern(lhs, parseCharRange());
				}
				m_input.skip(); // skip ']'
				return lhs;
			}


			NodePtr parseCharRange() {
				// save the position and value of the lower bound of the possible character range
				const auto rangeStart = m_input.loc();
				const auto low = parseChar();

				// no char range, just a single character
				auto e = atom(low);
				if(!m_input.skipIf('-')) {
					return e;
				}

				// string of the form 'l-h' is a character range
				if(m_input.peek() != ']') {
					const auto high = parseChar();
					if(low > high) {
						const auto rangeLoc = SourceLoc(
							rangeStart.startCol(),
							m_input.loc().pos() - rangeStart.pos(),
							rangeStart.lineNo(),
							rangeStart.linePos()
						);
						throw ParseError::outOfOrderCharRange(rangeLoc);
					}

					for(auto ch = low + 1; ch <= high; ) {
						e = altern(e, atom(ch++));
					}
				} else {
					e = altern(e, atom('-'));
				}

				return e;
			}


			char parseChar() {
				if(m_input.skipIf('\\')) {
					return parseEscapeSeq();
				}
				return m_input.get();
			}


			char parseEscapeSeq() {
				const auto ch = m_input.get();
				switch(ch) {
					case '0': return '\0';
					case 'a': return '\a';
					case 'b': return '\b';
					case 'f': return '\f';
					case 'n': return '\n';
					case 'r': return '\r';
					case 't': return '\t';
					case 'v': return '\v';
					case 'x': {
						if(char_utils::isHexDigit(m_input.peek())) {
							auto ch = char_utils::evalHexDigit(m_input.get());
							if(!m_input.isEof() && char_utils::isHexDigit(m_input.peek())) {
								ch = ch * 16 + char_utils::evalHexDigit(m_input.get());
							}
							return static_cast<char>(ch);
						}
						throw ParseError::emptyHexCharSeq(m_input.loc());
					}
					default: return ch;
				}
			}


			TextScanner m_input;
		};
	}


	NodePtr Parser::parse(std::string_view regex) {
		auto input = std::istringstream(std::string(regex));
		return parse(input);
	}


	NodePtr Parser::parse(std::istream& input) {
		return ParserImpl(input).run();
	}


	bool Parser::isMetaChar(char ch) {
		switch(ch) {
			case '|':
			case '*': case '?': case '+':
			case '(': case ')':
			case '[': case ']':
			case '\\': {
				return true;
			}
			default: {
				return false;
			}
		}
	}
}