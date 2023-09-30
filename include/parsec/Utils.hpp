#ifndef PARSEC_UTILS_HEADER
#define PARSEC_UTILS_HEADER

#include <gsl/gsl>

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
	 * @brief Describes a location in the source code.
	 */
	class SourceLocation {
	public:
		/** @{ */
		/** @brief Construct a new empty SourceLocation. */
		constexpr SourceLocation() = default;

		/** @brief Construct a new SourceLocation from its components. */
		constexpr SourceLocation(
			gsl::index startPos,
			gsl::index colNo,
			std::size_t colCount,
			gsl::index lineNo
		) noexcept
		 : startPos(startPos), colNo(colNo), colCount(colCount), lineNo(lineNo)
		{ }

		/** @brief Destroy the SourceLocation. */
		constexpr ~SourceLocation() = default;
		/** @} */

		/** @{ */
		/** @brief Construct a new SourceLocation by making a copy of another SourceLocation. */
		constexpr SourceLocation(const SourceLocation&) = default;

		/** @brief Assign to the SourceLocation a copy of another SourceLocation. */
		constexpr SourceLocation& operator=(const SourceLocation&) = default;
		/** @} */

		/** @{ */
		/** @brief Get the starting position of the SourceLocation. */
		constexpr gsl::index GetStartPos() const noexcept {
			return startPos;
		}

		/** @brief Get the starting column of the SourceLocation relative to its starting position. */
		constexpr gsl::index GetColumnNo() const noexcept {
			return colNo;
		}
		/** @brief Get the column count in the SourceLocation. */
		constexpr std::size_t GetColumnCount() const noexcept {
			return colCount;
		}

		/** @brief Get the line containing the SourceLocation. */
		constexpr gsl::index GetLineNo() const noexcept {
			return lineNo;
		}
		/** @} */

	private:
		gsl::index startPos;
		gsl::index colNo;
		std::size_t colCount;
		gsl::index lineNo;
	};



	/**
	 * @brief Indicates a syntax error in the source code.
	 */
	class ParseError : public std::runtime_error {
	public:
		/** @{ */
		/** @brief Construct a new ParseError from a C-style string message and a location. */
		ParseError(gsl::czstring msg, const SourceLocation& loc) noexcept
		 : runtime_error(msg), loc(loc)
		{ }

		/** @brief Construct a new ParseError from a string message and a location. */
		ParseError(const std::string& msg, const SourceLocation& loc) noexcept
		 : runtime_error(msg), loc(loc)
		{ }

		/** @brief Destroy the ParseError. */
		~ParseError() = default;
		/** @} */

		/** @{ */
		/** @brief Construct a new ParseError by making a copy of another ParseError. */
		ParseError(const ParseError&) = default;

		/** @brief Assign to the ParseError a copy of another ParseError object. */
		ParseError& operator=(const ParseError&) = default;
		/** @} */

		/** @{ */
		/** @brief Get a location in the source code where the ParseError occurred. */
		const SourceLocation& location() const noexcept {
			return loc;
		}
		/** @} */

	private:
		SourceLocation loc;
	};
}

#endif