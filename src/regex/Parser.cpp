#include "regex/Parser.hpp"

#include "utils/TextScanner.hpp"
#include "utils/Error.hpp"
#include "utils/chars.hpp"

#include "fg/rules.hpp"

#include <gsl/narrow>
#include <sstream>

namespace parsec::regex {
	namespace {
		class ParseImpl {
		public:
			explicit ParseImpl(std::istream& input) noexcept
				: m_scanner(input)
			{ }
			
			fg::RulePtr operator()() {
				// no input, nothing to parse
				if(m_scanner.eof()) {
					return fg::makeRule<fg::NilRule>();
				}

				// parse the input if there is any
				auto e = parseExpr();
				if(!m_scanner.eof()) {
					unexpected();
				}
				return e;
			}

		private:
			/** @{ */
			[[noreturn]] void unexpected() const {
				throw Error("unexpected '"
						+ escapeChar(m_scanner.peek())
						+ '\'',
					m_scanner.loc()
				);
			}
			
			[[noreturn]] void unmatchedParen(const SourceLoc& loc) const {
				throw Error("unmatched parenthesis", loc);
			}

			[[noreturn]] void invalidHexSeq() const {
				throw Error(
					"expected at least one hexadecimal digit",
					m_scanner.loc()
				);
			}

			[[noreturn]] void rangeBadOrder(const SourceLoc& loc) const {
				throw Error(
					"character range is out of order", {
						loc.startCol(),
						m_scanner.pos() - loc.pos(),
						loc.lineNo(),
						loc.linePos()
					}
				);
			}

			[[noreturn]] void invalidEscapeSeq() const {
				const auto loc = m_scanner.loc();
				throw Error(
					"invalid escape sequence", {
						loc.startCol() - 1,
						2,
						loc.lineNo(),
						loc.linePos()
					}
				);
			}
			/** @} */


			/** @{ */
			bool atomStart() const {
				if(!m_scanner.eof()) {
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
					case '*': case '|': case '(': case ')': case '[': case ']': case '?': case '+':
					case '\\': case '\'': case '\"': { return m_scanner.get(); }
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
							if(!m_scanner.eof() && isHexDigit(m_scanner.peek())) {
								ch = ch * 16 + evalHexDigit(m_scanner.get());
							}
							return gsl::narrow_cast<char>(ch);
						}
						invalidHexSeq();
					}
				}
				invalidEscapeSeq();
			}
			
			char parseCharLiteral() {
				if(m_scanner.skipIf('\\')) {
					return parseEscapeSeq();
				}
				return m_scanner.get();
			}
			/** @} */


			/** @{ */
			fg::RulePtr parseCharRange() {
				// save the position and value of the lower bound of the possible character range
				const auto rangeLoc = m_scanner.loc();
				const auto low = parseCharLiteral();

				// no char range, just a single character
				auto r = fg::makeRule<fg::Atom>(low);
				if(!m_scanner.skipIf('-')) {
					return r;
				}

				// string of the form 'l-h' is a character range
				if(m_scanner.peek() != ']') {
					const auto high = parseCharLiteral();
					if(low > high) {
						rangeBadOrder(rangeLoc);
					}

					for(auto ch = low; ch <= high; ) {
						r = fg::makeRule<fg::RuleAltern>(
							std::move(r),
							fg::makeRule<fg::Atom>(ch++)
						);
					}
				} else {
					r = fg::makeRule<fg::RuleAltern>(
						std::move(r),
						fg::makeRule<fg::Atom>('-')
					);
				}

				return r;
			}
			
			fg::RulePtr parseCharSet() {
				// empty character set
				if(m_scanner.skipIf(']')) {
					return fg::makeRule<fg::NilRule>();
				}

				auto lhs = parseCharRange();
				while(m_scanner.peek() != ']') {
					lhs = fg::makeRule<fg::RuleAltern>(
						std::move(lhs),
						parseCharRange()
					);
				}
				m_scanner.skip(); // skip ']'
				return lhs;
			}
			/** @} */


			/** @{ */
			fg::RulePtr parseAtom() {
				if(!atomStart()) {
					unexpected();
				}

				if(const auto openParen = m_scanner.loc(); m_scanner.skipIf('(')) {
					// empty parenthesized expression
					if(m_scanner.skipIf(')')) {
						return fg::makeRule<fg::NilRule>();
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

				return fg::makeRule<fg::Atom>(parseCharLiteral());
			}

			fg::RulePtr parseRepeat() {
				auto e = parseAtom();
				
				while(true) {
					if(m_scanner.skipIf('*')) {
						e = fg::makeRule<fg::StarRule>(std::move(e));
					} else if(m_scanner.skipIf('?')) {
						e = fg::makeRule<fg::OptionalRule>(std::move(e));
					} else if(m_scanner.skipIf('+')) {
						e = fg::makeRule<fg::PlusRule>(std::move(e));
					} else {
						break;
					}
				}

				return e;
			}

			fg::RulePtr parseConcat() {
				auto lhs = parseRepeat();
				while(atomStart()) {
					lhs = fg::makeRule<fg::RuleConcat>(
						std::move(lhs),
						parseRepeat()
					);
				}
				return lhs;
			}

			fg::RulePtr parseAltern() {
				auto lhs = parseConcat();
				while(m_scanner.skipIf('|')) {
					lhs = fg::makeRule<fg::RuleAltern>(
						std::move(lhs),
						parseConcat()
					);
				}
				return lhs;
			}
			
			fg::RulePtr parseExpr() {
				return parseAltern();
			}
			/** @} */


			TextScanner m_scanner;
		};
	}


	fg::RulePtr Parser::parse(std::string_view regex) {
		auto input = std::istringstream(std::string(regex));
		return parse(input);
	}

	fg::RulePtr Parser::parse(std::istream& input) {
		return ParseImpl(input)();
	}
}