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
	/** @brief Simple integer type for representing numeric identifiers. */
	using Uid = std::size_t;

	/**
	 * @brief Specifies the order in which child nodes are visited during tree traversal.
	 */
	enum class TraversalTypes {
		None, /**< @brief Child nodes are not visited. */
		Preorder, /**< @brief Root of a tree is visited first, then the child nodes. */
		Postorder /**< @brief Child nodes of a tree are visited first, then the root. */
	};



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

		/** @brief Represent a character in a human-readable form. */
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
}

#endif