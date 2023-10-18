#ifndef PARSEC_REGEX_PARSER_HEADER
#define PARSEC_REGEX_PARSER_HEADER

#include "../utils/TextScanner.hpp"

#include "ExprNode.hpp"
#include <memory>

namespace parsec::regex {
	/**
	 * @brief Parses regular expressions.
	 */
	class Parser {
	public:
		/** @{ */
		Parser() = default;
		/** @} */


		/** @{ */
		Parser(Parser&&) = default;
		Parser& operator=(Parser&&) = default;
		/** @} */


		/** @{ */
		Parser(const Parser&) = delete;
		Parser& operator=(const Parser&) = delete;
		/** @} */


		/** @{ */
		/** @brief Parses an arbitrary character sequence for a valid regular expression. */
		std::unique_ptr<ExprNode> parse(std::string_view regex);


		/** @brief Parses characters from a @c std::istream for a valid regular expression. */
		std::unique_ptr<ExprNode> parse(std::istream& input);
		/** @} */


	private:
		/** @{ */
		[[noreturn]] void badSyntax() const;
		/** @} */


		/** @{ */
		bool atomStart() const;
		/** @} */


		/** @{ */
		char parseEscapeSeq();
		char parseCharLiteral();
		void parseCharRange();
		void parseCharSet();

		void parseAtom();
		void parseRepeat();
		void parseConcat();
		void parseAltern();
		void parseExpr();
		/** @} */


		/** @} */
		std::unique_ptr<ExprNode> m_regex;
		TextScanner m_input;
		/** @} */
	};
}

#endif