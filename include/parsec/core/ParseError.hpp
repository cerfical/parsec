#ifndef PARSEC_CORE_PARSE_ERROR_HEADER
#define PARSEC_CORE_PARSE_ERROR_HEADER

#include "SourceLoc.hpp"
#include "Error.hpp"

namespace parsec {

	class ParseError : public Error {
	public:

		[[nodiscard]]
		static ParseError nameRedefine(const SourceLoc& loc);
		
		[[nodiscard]]
		static ParseError reservedNameRedefine(const SourceLoc& loc);

		[[nodiscard]]
		static ParseError emptyName(const SourceLoc& loc);

		[[nodiscard]]
		static ParseError undefinedName(const SourceLoc& loc);

		[[nodiscard]]
		static ParseError patternConflict(const SourceLoc& loc, std::string_view pattern);
	
		[[nodiscard]]
		static ParseError ruleConflict(const SourceLoc& loc, std::string_view rule);

		[[nodiscard]]
		static ParseError unexpectedEof(const SourceLoc& loc);

		[[nodiscard]]
		static ParseError invalidChar(const SourceLoc& loc, char ch);

		[[nodiscard]]
		static ParseError emptyHexCharSeq(const SourceLoc& loc);

		[[nodiscard]]
		static ParseError outOfOrderCharRange(const SourceLoc& loc);

		[[nodiscard]]
		static ParseError misplacedChar(const SourceLoc& loc, char ch);

		[[nodiscard]]
		static ParseError misplacedToken(const SourceLoc& loc, std::string_view tok);

		[[nodiscard]]
		static ParseError unmatchedToken(const SourceLoc& loc, std::string_view expect, std::string_view got);


		explicit ParseError(const SourceLoc& loc)
			: ParseError(loc, "parse error") {}

		ParseError(const SourceLoc& loc, const std::string& msg)
			: Error(msg), m_loc(loc) {}


		const SourceLoc& loc() const {
			return m_loc;
		}


	private:
		SourceLoc m_loc;
	};

}

#endif