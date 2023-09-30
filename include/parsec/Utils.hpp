#ifndef PARSEC_UTILS_HEADER
#define PARSEC_UTILS_HEADER

#include <gsl/narrow>
#include <gsl/util>

#include <stdexcept>
#include <string_view>
#include <string>
#include <cctype>

namespace parsec {
	/**
	 * @brief Utility functions for working with characters.
	 */
	class CharUtils {
	public:
		/** @{ */
		/** @brief Safely convert a character to an integer. */
		constexpr static int CharToInt(char ch) noexcept {
			return static_cast<unsigned char>(ch);
		}

		/** @brief Retrieve an escape sequence for a character. */
		static std::string EscapeChar(char ch);
		/** @} */

		/** @{ */
		/** @brief Check if a character is an alphabetic character. */
		static bool IsAlpha(char ch) noexcept {
			return std::isalpha(CharToInt(ch));
		}

		/** @brief Check if a character is a digit. */
		static bool IsDigit(char ch) noexcept {
			return std::isdigit(CharToInt(ch));
		}

		/** @brief Check if a character is an alphanumeric character. */
		static bool IsAlnum(char ch) noexcept {
			return std::isalnum(CharToInt(ch));
		}

		/** @brief Check if a character is a whitespace character. */
		static bool IsSpace(char ch) noexcept {
			return std::isspace(CharToInt(ch));
		}
		
		/** @brief Check if a character is a printable character. */
		static bool IsPrint(char ch) noexcept {
			return std::isprint(CharToInt(ch));
		}
		/** @} */

	private:
		CharUtils() = delete;
		~CharUtils() = delete;
	};



	/**
	 * @brief Represents a sequence of characters between two positions in text.
	 */
	class CharRange {
	public:
		/**
		 * @brief Range defined by starting position and size.
		 */
		struct StartAndSize {
			gsl::index startPos = 0; /**< @brief Starting position of the range. */
			std::size_t size = 0; /**< @brief Size of the range. */
		};

		/**
		 * @brief Range defined by two endpoints.
		 */
		struct Endpoints {
			gsl::index startPos = 0; /**< @brief Starting position of the range. */
			gsl::index endPos = 0; /**< @brief End position of the range. */
		};

		/** @{ */
		/** @brief Construct an empty CharRange. */
		constexpr CharRange() = default;

		/** @brief Construct a CharRange from a range defined by starting position and size. */
		constexpr CharRange(StartAndSize range) noexcept
		 : start(range.startPos), size(range.size)
		{ }
		
		/** @brief Construct a CharRange from a range defined by two endpoints. */
		constexpr CharRange(Endpoints range) noexcept
		 : CharRange({
			   .startPos = range.startPos,
			   .size = gsl::narrow_cast<std::size_t>(range.endPos - range.startPos)
		   })
		{ }

		/** @brief Destroy CharRange. */
		constexpr ~CharRange() = default;
		/** @} */

		/** @{ */
		/** @brief Construct a CharRange from a copy of CharRange object. */
		constexpr CharRange(const CharRange&) = default;

		/** @brief Assign a copy of CharRange object to CharRange. */
		constexpr CharRange& operator=(const CharRange&) = default;
		/** @} */

		/** @{ */
		/** @brief Get starting position of CharRange. */
		constexpr gsl::index GetStartPos() const noexcept {
			return start;
		}

		/** @brief Get end position of CharRange. */
		constexpr gsl::index GetEndPos() const noexcept {
			return start + size;
		}

		/** @brief Get number of characters in CharRange. */
		constexpr std::size_t GetSize() const noexcept {
			return size;
		}

		/** @brief Check if CharRange is empty. */
		constexpr bool IsEmpty() const noexcept {
			return GetSize() == 0;
		}
		/** @} */
	
	private:
		gsl::index start = 0;
		std::size_t size = 0;
	};



	/**
	 * @brief Describes a location in the source code.
	 */
	class SourceLocation {
	public:
		/**
		 * @brief Contains location information.
		 */
		struct LocationInfo {
			CharRange chars = { }; /**< @brief Range of characters that the location spans. */
			gsl::index lineNo = 0; /**< @brief Line that contains the location. */
			gsl::index colNo = 0; /**< @brief Column that starts the location. */
		};

		/** @{ */
		/** @brief Construct an empty SourceLocation. */
		constexpr SourceLocation() = default;

		/** @brief Construct a SourceLocation from its description. */
		constexpr SourceLocation(const LocationInfo& loc) noexcept
		 : loc(loc)
		{ }

		/** @brief Destroy SourceLocation. */
		constexpr ~SourceLocation() = default;
		/** @} */

		/** @{ */
		/** @brief Construct a SourceLocation from a copy of SourceLocation object. */
		constexpr SourceLocation(const SourceLocation&) = default;
		
		/** @brief Assign a copy of SourceLocation object to SourceLocation. */
		constexpr SourceLocation& operator=(const SourceLocation&) = default;
		/** @} */

		/** @{ */
		/** @brief Get character sequence backing SourceLocation. */
		constexpr CharRange GetChars() const noexcept {
			return loc.chars;
		}

		/** @brief Get line number of SourceLocation. */
		constexpr gsl::index GetLineNo() const noexcept {
			return loc.lineNo;
		}

		/** @brief Get column number of SourceLocation. */
		constexpr gsl::index GetColNo() const noexcept {
			return loc.colNo;
		}

		/** @brief Get starting position of SourceLocation. */
		constexpr gsl::index GetStartPos() const noexcept {
			return loc.chars.GetStartPos();
		}

		/** @brief Get end position of SourceLocation. */
		constexpr gsl::index GetEndPos() const noexcept {
			return loc.chars.GetEndPos();
		}

		/** @brief Get number of characters in SourceLocation. */
		constexpr std::size_t GetSize() const noexcept {
			return loc.chars.GetSize();
		}

		/** @brief Check if SourceLocation is empty. */
		constexpr bool IsEmpty() const noexcept {
			return loc.chars.IsEmpty();
		}
		/** @} */

	private:
		LocationInfo loc = { };
	};



	/**
	 * @brief Indicates a syntax error in the source code.
	 */
	class ParseError : public std::runtime_error {
	public:
		/** @{ */
		/** @brief Construct a ParseError from a C-style string message and a location. */
		ParseError(const char* msg, const SourceLocation& loc) noexcept
		 : runtime_error(msg), loc(loc)
		{ }

		/** @brief Construct a ParseError from a string message and a location. */
		ParseError(const std::string& msg, const SourceLocation& loc) noexcept
		 : runtime_error(msg), loc(loc)
		{ }

		/** @brief Destroy ParseError. */
		~ParseError() = default;
		/** @} */

		/** @{ */
		/** @brief Construct a ParseError from a copy of ParseError object. */
		ParseError(const ParseError&) = default;

		/** @brief Assign a copy of ParseError object to ParseError. */
		ParseError& operator=(const ParseError&) = default;
		/** @} */

		/** @{ */
		/** @brief Get location in the source code where ParseError occurred. */
		const SourceLocation& location() const noexcept {
			return loc;
		}
		/** @} */

	private:
		SourceLocation loc;
	};
}

#endif