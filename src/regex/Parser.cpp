#include "regex/Parser.hpp"
#include "regex/nodes.hpp"

#include "utils/ParseError.hpp"
#include "utils/chars.hpp"

#include <gsl/gsl>
#include <sstream>

namespace parsec::regex {
	void Parser::badSyntax() const {
		const auto ch = m_input.peek();
		const auto msg = (std::ostringstream()
			<< "unexpected \'" << ch << '\''
		).str();

		throw ParseError(msg, m_input.pos());
	}


	bool Parser::atomStart() const {
		if(!m_input.eof()) {
			switch(m_input.peek()) {
				case '*': case '|': case ')': case ']': case '?': case '+': break;
				case '(': case '[': case '\\': default: return true;
			}	
		}
		return false;
	}


	char Parser::parseEscapeSeq() {
		switch(m_input.peek()) {
			case '*': case '|': case '(': case ')': case '[': case ']': case '?': case '+':
			case '\\': case '\'': case '\"': { return m_input.get(); }
			case '0': { m_input.skip(); return '\0'; }
			case 'a': { m_input.skip(); return '\a'; }
			case 'b': { m_input.skip(); return '\b'; }
			case 'f': { m_input.skip(); return '\f'; }
			case 'n': { m_input.skip(); return '\n'; }
			case 'r': { m_input.skip(); return '\r'; }
			case 't': { m_input.skip(); return '\t'; }
			case 'v': { m_input.skip(); return '\v'; }
			case 'x': {
				m_input.skip();
				if(isHexDigit(m_input.peek())) {
					auto ch = evalHexDigit(m_input.get());
					if(!m_input.eof() && isHexDigit(m_input.peek())) {
						ch = ch * 16 + evalHexDigit(m_input.get());
					}
					return gsl::narrow_cast<char>(ch);
				}
				throw ParseError("expected at least one hexadecimal digit", m_input.pos());
			}
		}
		throw ParseError("invalid escape sequence", SourceLoc(m_input.pos() - 1, 2));
	}
	
	char Parser::parseCharLiteral() {
		if(m_input.skipIf('\\')) {
			return parseEscapeSeq();
		} 
		return m_input.get();
	}

	void Parser::parseCharRange() {
		const auto startPos = m_input.pos();
		const auto low = parseCharLiteral();

		std::unique_ptr<ExprNode> e = std::make_unique<CharLiteral>(low);
		if(m_input.skipIf('-')) {
			if(m_input.peek() != ']') {
				const auto high = parseCharLiteral();
				if(low > high) {
					throw ParseError(
						"character range is out of order",
						SourceLoc(startPos, m_input.pos() - startPos)
					);
				}

				for(char ch = low; ch < high; ) {
					auto right = std::make_unique<CharLiteral>(++ch);
					e = std::make_unique<AlternExpr>(std::move(e), std::move(right));
				}
			} else {
				e = std::make_unique<AlternExpr>(std::move(e), std::make_unique<CharLiteral>('-'));
			}
		}

		m_regex = std::move(e);
	}

	void Parser::parseCharSet() {
		if(!m_input.skipIf(']')) {
			auto left = (parseCharRange(), std::move(m_regex));
			while(m_input.peek() != ']') {
				auto right = (parseCharRange(), std::move(m_regex));
				left = std::make_unique<AlternExpr>(std::move(left), std::move(right));
			}
			m_input.skip(); // skip ']'
			m_regex = std::move(left);
		} else {
			m_regex = std::make_unique<NilExpr>();
		}
	}


	void Parser::parseAtom() {
		if(!atomStart()) {
			badSyntax();
		}
		
		if(const auto startPos = m_input.pos(); m_input.skipIf('(')) {
			if(!m_input.skipIf(')')) {
				parseExpr();
				if(!m_input.skipIf(')')) {
					throw ParseError("unmatched parenthesis", startPos);
				}
			} else {
				m_regex = std::make_unique<NilExpr>();
			}
		} else if(m_input.skipIf('[')) {
			parseCharSet();
		} else {
			m_regex = std::make_unique<CharLiteral>(parseCharLiteral());
		}
	}

	void Parser::parseRepeat() {
		auto e = (parseAtom(), std::move(m_regex));
		while(true) {
			if(m_input.skipIf('*')) {
				e = std::make_unique<StarExpr>(std::move(e));
			} else if(m_input.skipIf('?')) {
				e = std::make_unique<OptionalExpr>(std::move(e));
			} else if(m_input.skipIf('+')) {
				e = std::make_unique<PlusExpr>(std::move(e));
			} else {
				break;
			}
		}
		m_regex = std::move(e);
	}

	void Parser::parseConcat() {
		auto left = (parseRepeat(), std::move(m_regex));
		while(atomStart()) {
			auto right = (parseRepeat(), std::move(m_regex));
			left = std::make_unique<ConcatExpr>(std::move(left), std::move(right));
		}
		m_regex = std::move(left);
	}
	
	void Parser::parseAltern() {
		auto left = (parseConcat(), std::move(m_regex));
		while(m_input.skipIf('|')) {
			auto right = (parseConcat(), std::move(m_regex));
			left = std::make_unique<AlternExpr>(std::move(left), std::move(right));
		}
		m_regex = std::move(left);
	}

	void Parser::parseExpr() {
		parseAltern();
	}


	std::unique_ptr<ExprNode> Parser::parse(std::string_view regex) {
		auto input = std::istringstream(std::string(regex));
		return parse(input);
	}

	std::unique_ptr<ExprNode> Parser::parse(std::istream& input) {
		// ensure the input stream is reset after the parse is complete
		const auto sentry = gsl::finally([this]() {
			m_input = TextScanner();
		});
		m_input = TextScanner(input);
		
		// parse the expression if there is any input
		if(!m_input.eof()) {
			parseExpr();
			if(!m_input.eof()) {
				if(m_input.peek() == ')') {
					throw ParseError("unmatched parenthesis", m_input.pos());
				}
				badSyntax();
			}
		} else {
			m_regex = std::make_unique<NilExpr>();
		}

		return std::move(m_regex);
	}
}