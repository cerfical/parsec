#ifndef PARSEC_UTILS_HEADER
#define PARSEC_UTILS_HEADER

#include <stdexcept>
#include <string_view>
#include <string>
#include <cctype>

namespace parsec {
	/** @brief Represents non-negative size values. */
	using Size = std::ptrdiff_t;
	/** @brief Represents non-negative index values. */
	using Index = std::ptrdiff_t;



	/**
	 * @brief Utility functions for working with characters.
	 */
	class CharUtils {
	public:
		/** @name Character conversions: */

		/** @{ */
		/** @brief Safely converts a character to an integer. */
		constexpr static int CharToInt(char ch) noexcept {
			return static_cast<unsigned char>(ch);
		}

		/** @brief Returns an escape sequence for a character. */
		static std::string EscapeChar(char ch);
		/** @} */

		/** @name Retrieving character types: */

		/** @{ */
		/** @brief Checks if a character is an alphabetic character. */
		static bool IsAlpha(char ch) noexcept {
			return std::isalpha(CharToInt(ch));
		}

		/** @brief Checks if a character is a digit. */
		static bool IsDigit(char ch) noexcept {
			return std::isdigit(CharToInt(ch));
		}

		/** @brief Checks if a character is an alphanumeric character. */
		static bool IsAlnum(char ch) noexcept {
			return std::isalnum(CharToInt(ch));
		}

		/** @brief Checks if a character is a whitespace character. */
		static bool IsSpace(char ch) noexcept {
			return std::isspace(CharToInt(ch));
		}
		
		/** @brief Checks if a character is a printable character. */
		static bool IsPrint(char ch) noexcept {
			return std::isprint(CharToInt(ch));
		}
		/** @} */

	private:
		CharUtils() = delete;
		~CharUtils() = delete;
	};



	/**
	 * @brief Represents a sequence of characters between two positions in some text.
	 */
	class CharRange {
	public:
		/**
		 * @brief Specifies the start position and size of the range.
		 */
		struct StartAndSize {
			Index startPos = 0; /**< @brief Start position of a range. */
			Size size = 0; /**< @brief Size of a range. */
		};

		/**
		 * @brief Specifies the start and end positions of the range.
		 */
		struct Endpoints {
			Index startPos = 0; /**< @brief Start position of a range. */
			Index endPos = 0; /**< @brief End position of a range. */
		};

		/** @name Object construction/destruction: */

		/** @{ */
		constexpr CharRange() = default;

		/** @brief Constructs a range from a start position and size. */
		constexpr CharRange(StartAndSize range) noexcept
		 : start(range.startPos), size(range.size)
		{ }
		
		/** @brief Constructs a range from two endpoints. */
		constexpr CharRange(Endpoints range) noexcept
		 : CharRange({ .startPos = range.startPos, .size = range.endPos - range.startPos})
		{ }

		constexpr ~CharRange() = default;
		/** @} */

		/** @name Copy operations: */

		/** @{ */
		constexpr CharRange(const CharRange&) = default;
		constexpr CharRange& operator=(const CharRange&) = default;
		/** @} */

		/** @{ */
		/** @brief Returns the range's start position. */
		constexpr Index GetStartPos() const noexcept {
			return start;
		}

		/** @brief Returns the range's end position. */
		constexpr Index GetEndPos() const noexcept {
			return start + size;
		}

		/** @brief Returns the range's size. */
		constexpr Size GetSize() const noexcept {
			return size;
		}

		/** @brief Checks if the range is empty. */
		constexpr bool IsEmpty() const noexcept {
			return GetSize() == 0;
		}
		/** @} */
	
	private:
		Index start = 0;
		Size size = 0;
	};



	/**
	 * @brief Describes a location in some source code.
	 */
	class SourceLocation {
	public:
		/**
		 * @brief Contains information about a location.
		 */
		struct LocationInfo {
			CharRange chars = { }; /**< @brief The range of characters that the location covers. */
			Index lineNo = 0; /**< @brief The line number that contains the location. */
		};

		/** @name Object construction/destruction: */

		/** @{ */
		constexpr SourceLocation() = default;

		/** @brief Constructs a location from its description. */
		constexpr SourceLocation(const LocationInfo& loc) noexcept
		 : loc(loc)
		{ }

		constexpr ~SourceLocation() = default;
		/** @} */

		/** @name Copy operations: */

		/** @{ */
		constexpr SourceLocation(const SourceLocation&) = default;
		constexpr SourceLocation& operator=(const SourceLocation&) = default;
		/** @} */

		/** @{ */
		/** @brief Returns the character sequence representing the location. */
		constexpr CharRange GetChars() const noexcept {
			return loc.chars;
		}

		/** @brief Returns the line number containing the location. */
		constexpr Index GetLineNo() const noexcept {
			return loc.lineNo;
		}

		/** @brief Returns the start position of the location. */
		constexpr Index GetStartPos() const noexcept {
			return loc.chars.GetStartPos();
		}

		/** @brief Returns the end position of the location. */
		constexpr Index GetEndPos() const noexcept {
			return loc.chars.GetEndPos();
		}

		/** @brief Returns the location size. */
		constexpr Size GetSize() const noexcept {
			return loc.chars.GetSize();
		}

		/** @brief Checks if the location is empty. */
		constexpr bool IsEmpty() const noexcept {
			return loc.chars.IsEmpty();
		}
		/** @} */

	private:
		LocationInfo loc = { };
	};



	/**
	 * @brief Represents a syntax error in some text.
	 */
	class ParseError : public std::runtime_error {
	public:
		/** @name Object construction/destruction: */

		/** @{ */
		/** @brief Constructs an error from a C-style string message and a location. */
		ParseError(const char *msg, const SourceLocation& loc) noexcept
		 : runtime_error(msg), loc(loc)
		{ }

		/** @brief Constructs an error from a string message and a location. */
		ParseError(const std::string &msg, const SourceLocation& loc) noexcept
		 : runtime_error(msg), loc(loc)
		{ }

		~ParseError() = default;
		/** @} */

		/** @name Copy operations: */
		
		/** @{ */
		ParseError(const ParseError&) = default;
		ParseError& operator=(const ParseError&) = default;
		/** @} */

		/** @{ */
		/** @brief Returns the location in text where the error occurred. */
		const SourceLocation& location() const noexcept {
			return loc;
		}
		/** @} */

	private:
		SourceLocation loc;
	};
}

#endif