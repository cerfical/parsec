#ifndef PARSEC_UTILS_HEADER
#define PARSEC_UTILS_HEADER

#include <gsl/gsl>

#include <stdexcept>
#include <string_view>
#include <iostream>
#include <sstream>
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
		/** @brief Construct a new empty location. */
		constexpr SourceLocation() = default;

		/** @brief Construct a new location from its description. */
		constexpr SourceLocation(
			gsl::index startPos,
			gsl::index colNo,
			std::size_t colCount,
			gsl::index lineNo
		) noexcept
		 : startPos(startPos), colNo(colNo), colCount(colCount), lineNo(lineNo)
		{ }

		/** @copybrief */
		constexpr ~SourceLocation() = default;
		/** @} */


		/** @{ */
		constexpr SourceLocation(const SourceLocation&) = default;
		constexpr SourceLocation& operator=(const SourceLocation&) = default;
		/** @} */


		/** @{ */
		/** @brief Get the starting position of the location. */
		constexpr gsl::index GetStartPos() const noexcept {
			return startPos;
		}

		/** @brief Get the starting column of the location relative to its starting position. */
		constexpr gsl::index GetColumnNo() const noexcept {
			return colNo;
		}
		/** @brief Get the column count spanning the location. */
		constexpr std::size_t GetColumnCount() const noexcept {
			return colCount;
		}

		/** @brief Get the line containing the location. */
		constexpr gsl::index GetLineNo() const noexcept {
			return lineNo;
		}

		/** @brief Check if the location is an empty location. */
		constexpr bool IsEmpty() const noexcept {
			return GetColumnCount() == 0;
		}
		/** @} */

	private:
		gsl::index startPos = { };
		gsl::index colNo = { };
		std::size_t colCount = 0;
		gsl::index lineNo = { };
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

		/** @copybrief */
		~ParseError() = default;
		/** @} */
		

		/** @{ */
		ParseError(const ParseError&) = default;
		ParseError& operator=(const ParseError&) = default;
		/** @} */


		/** @{ */
		/** @brief Get a location in the source code where the error occurred. */
		const SourceLocation& location() const noexcept {
			return loc;
		}
		/** @} */

	private:
		SourceLocation loc;
	};



	/**
	 * @brief Performs error logging with additional ability to augment error messages with contextual information.
	 */
	class ErrorLogger {
	public:
		/** @{ */
		/** @brief Construct a new logger with the specified file as the context for the logger. */
		ErrorLogger(std::istream& input, std::string_view filename, std::ostream& out = std::cerr) noexcept
		 : filename(filename), input(&input), out(&out)
		{ }

		/** @copybrief */
		~ErrorLogger() = default;
		/** @} */

		/** @{ */
		ErrorLogger(ErrorLogger&&) = default;
		ErrorLogger& operator=(ErrorLogger&&) = default;
		/** @} */

		/** @{ */
		ErrorLogger(const ErrorLogger&) = delete;
		ErrorLogger& operator=(const ErrorLogger&) = delete;
		/** @} */

		/** @{ */
		/** @brief Print out the error message and optionally its origins. */
		void Log(std::string_view msg, const SourceLocation& loc = { }) noexcept;
		/** @} */

	private:
		/** @brief Read the line defined by a location from the input. */
		std::string ReadLine(const SourceLocation& loc);

		std::string_view filename;
		std::istream* input = nullptr;
		
		std::ostream* out = nullptr;
	};
}

#endif