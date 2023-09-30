#ifndef PARSEC_UTILS_HEADER
#define PARSEC_UTILS_HEADER

#include <gsl/gsl>

#include <stdexcept>
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

		/** @brief Find the value of a single digit represented as a character. */
		constexpr static int EvalDigit(char ch) noexcept {
			return ch - '0';
		}

		/** @brief Find the value of a single hexadecimal digit represented as a character. */
		constexpr static int EvalHexDigit(char ch) noexcept {
			return (ch >= 'a') ? ch - 'a' + 10 : ((ch >= 'A') ? ch - 'A' : EvalDigit(ch));
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

		/** @brief Check if a character is a hexadecimal digit. */
		static bool IsHexDigit(char ch) noexcept {
			return std::isxdigit(CharToInt(ch));
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
	struct SourceLoc {
	public:
		/** @brief Print out a SourceLoc to a @c std::ostream. */
		friend std::ostream& operator<<(std::ostream& out, const SourceLoc& loc);

		int startPos = { }; /**< @brief Starting position of the line containing the location. */
		int colNo = { }; /**< @brief Ordinal number of the location starting character. */
		int colCount = 0; /**< @brief Number of characters spanning the location. */
		int lineNo = { }; /**< @brief Line number containing the location. */
	};



	/**
	 * @brief Indicates a syntax error in the source code.
	 */
	class ParseError : public std::runtime_error {
	public:
		/** @{ */
		/** @brief Construct a new ParseError from a C-style string message and a location. */
		ParseError(gsl::czstring msg, const SourceLoc& loc)
		 : runtime_error(msg), m_loc(loc)
		{ }

		/** @brief Construct a new ParseError from a string message and a location. */
		ParseError(const std::string& msg, const SourceLoc& loc)
		 : runtime_error(msg), m_loc(loc)
		{ }
		/** @} */
		
		/** @{ */
		/** @brief Location in the source code where the error occurred. */
		const SourceLoc& GetLocation() const noexcept {
			return m_loc;
		}
		/** @} */

	private:
		SourceLoc m_loc = { };
	};
}

#endif