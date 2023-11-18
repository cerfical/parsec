#include "regex/Parser.hpp"
#include "regex/nodes.hpp"

#include "utils/chars.hpp"

#include "TextScanner.hpp"
#include "Error.hpp"

#include <gsl/narrow>
#include <sstream>
#include <format>

namespace parsec::regex {
	namespace {
		class ParseImpl {
		public:
			explicit ParseImpl(std::istream& input) noexcept
				: m_scanner(input)
			{ }
			
			ExprPtr operator()() {
				// no input, nothing to parse
				if(m_scanner.isEof()) {
					return makeExpr<NilExpr>();
				}

				auto e = parseExpr();
				if(!m_scanner.isEof()) {
					unexpected();
				}
				return e;
			}

		private:
			/** @{ */
			[[noreturn]] void unexpected() const {
				throw Error(std::format("unexpected '{}'",
						escapeChar(m_scanner.peek())
					),
					m_scanner.loc()
				);
			}
			
			[[noreturn]] void unmatchedParen(const SourceLoc& loc) const {
				throw Error("unmatched parenthesis", loc);
			}

			[[noreturn]] void invalidHexSeq() const {
				throw Error("expected at least one hexadecimal digit",
					m_scanner.loc()
				);
			}

			[[noreturn]] void rangeBadOrder(const SourceLoc& loc) const {
				throw Error("character range is out of order", {
					loc.startCol(),
					m_scanner.pos() - loc.pos(),
					loc.lineNo(),
					loc.linePos()
				});
			}
			/** @} */


			/** @{ */
			bool isAtom() const {
				if(!m_scanner.isEof()) {
					switch(m_scanner.peek()) {
						case '*': case '|': case ')': case ']': case '?': case '+': break;
						case '(': case '[': case '\\': default: return true;
					}
				}
				return false;
			}
			/** @} */


			/** @{ */
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
						if(isHexDigit(m_scanner.peek())) {
							auto ch = evalHexDigit(m_scanner.get());
							if(!m_scanner.isEof() && isHexDigit(m_scanner.peek())) {
								ch = ch * 16 + evalHexDigit(m_scanner.get());
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
			/** @} */


			/** @{ */
			ExprPtr parseCharRange() {
				// save the position and value of the lower bound of the possible character range
				const auto rangeLoc = m_scanner.loc();
				const auto low = parseChar();

				// no char range, just a single character
				auto e = makeExpr<CharAtom>(low);
				if(!m_scanner.skipIf('-')) {
					return e;
				}

				// string of the form 'l-h' is a character range
				if(m_scanner.peek() != ']') {
					const auto high = parseChar();
					if(low > high) {
						rangeBadOrder(rangeLoc);
					}

					for(auto ch = low; ch <= high; ) {
						e = makeExpr<AlternExpr>(
							std::move(e),
							makeExpr<CharAtom>(ch++)
						);
					}
				} else {
					e = makeExpr<AlternExpr>(
						std::move(e),
						makeExpr<CharAtom>('-')
					);
				}

				return e;
			}
			
			ExprPtr parseCharSet() {
				// empty character set
				if(m_scanner.skipIf(']')) {
					return makeExpr<NilExpr>();
				}

				auto lhs = parseCharRange();
				while(m_scanner.peek() != ']') {
					lhs = makeExpr<AlternExpr>(
						std::move(lhs),
						parseCharRange()
					);
				}
				m_scanner.skip(); // skip ']'
				return lhs;
			}
			/** @} */


			/** @{ */
			ExprPtr parseAtom() {
				if(!isAtom()) {
					unexpected();
				}

				if(const auto openParen = m_scanner.loc(); m_scanner.skipIf('(')) {
					// empty parenthesized expression
					if(m_scanner.skipIf(')')) {
						return makeExpr<NilExpr>();
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

				return makeExpr<CharAtom>(parseChar());
			}

			ExprPtr parseRepeat() {
				auto e = parseAtom();
				while(true) {
					if(m_scanner.skipIf('*')) {
						e = makeExpr<StarExpr>(std::move(e));
					} else if(m_scanner.skipIf('?')) {
						e = makeExpr<OptionalExpr>(std::move(e));
					} else if(m_scanner.skipIf('+')) {
						e = makeExpr<PlusExpr>(std::move(e));
					} else {
						break;
					}
				}
				return e;
			}

			ExprPtr parseConcat() {
				auto lhs = parseRepeat();
				while(isAtom()) {
					lhs = makeExpr<ConcatExpr>(
						std::move(lhs),
						parseRepeat()
					);
				}
				return lhs;
			}

			ExprPtr parseAltern() {
				auto lhs = parseConcat();
				while(m_scanner.skipIf('|')) {
					lhs = makeExpr<AlternExpr>(
						std::move(lhs),
						parseConcat()
					);
				}
				return lhs;
			}
			
			ExprPtr parseExpr() {
				return parseAltern();
			}
			/** @} */


			TextScanner m_scanner;
		};
	}


	bool Parser::isMetachar(char ch) noexcept {
		switch(ch) {
			case '|':
			case '*': case '?': case '+':
			case '(': case ')':
			case '[': case ']':
				return true;
			default:
				return false;
		}
	}


	ExprPtr Parser::parse(std::string_view regex) {
		auto input = std::istringstream(std::string(regex));
		return parse(input);
	}

	ExprPtr Parser::parse(std::istream& input) {
		return ParseImpl(input)();
	}
}