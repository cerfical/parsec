#ifndef PARSEC_UTILS_CHARS_HEADER
#define PARSEC_UTILS_CHARS_HEADER

#include <string>
#include <cctype>

namespace parsec {
	/** @brief Safely convert a character to an integer. */
	constexpr inline int charToInt(char ch) noexcept {
		return static_cast<unsigned char>(ch);
	}


	/** @brief Find the value of a single digit represented as a character. */
	constexpr inline int evalDigit(char ch) noexcept {
		return ch - '0';
	}


	/** @brief Find the value of a single hexadecimal digit represented as a character. */
	constexpr inline int evalHexDigit(char ch) noexcept {
		return (ch >= 'a') ? ch - 'a' + 10 : ((ch >= 'A') ? ch - 'A' : evalDigit(ch));
	}


	/** @brief Present a character in a human-readable form. */
	std::string escapeChar(char ch);



	/** @brief Checks if a character is an alphabetic character. */
	inline bool isAlpha(char ch) noexcept {
		return std::isalpha(charToInt(ch));
	}


	/** @brief Checks if a character is a digit. */
	inline bool isDigit(char ch) noexcept {
		return std::isdigit(charToInt(ch));
	}


	/** @brief Checks if a character is a hexadecimal digit. */
	inline bool isHexDigit(char ch) noexcept {
		return std::isxdigit(charToInt(ch));
	}


	/** @brief Checks if a character is an alphanumeric character. */
	inline bool isAlnum(char ch) noexcept {
		return std::isalnum(charToInt(ch));
	}


	/** @brief Checks if a character is a whitespace character. */
	inline bool isSpace(char ch) noexcept {
		return std::isspace(charToInt(ch));
	}


	/** @brief Checks if a character is a printable character. */
	inline bool isPrint(char ch) noexcept {
		return std::isprint(charToInt(ch));
	}
}

#endif