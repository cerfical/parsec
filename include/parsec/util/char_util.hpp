#pragma once

#include <string>
#include <cctype>

/**
 * @brief Utilities for working with characters.
*/
namespace parsec::char_util {
		
	/**
	 * @brief Safely convert a character to an integer.
	*/
	constexpr inline int toInt(char ch) noexcept {
		return static_cast<unsigned char>(ch);
	}


	
	/**
	 * @brief Find the value of a digit character.
	*/
	constexpr inline int evalDigit(char ch) noexcept {
		return ch - '0';
	}



	/**
	 * @brief Find the value of a hexadecimal digit character.
	*/
	constexpr inline int evalHexDigit(char ch) noexcept {
		return (ch >= 'a') ? ch - 'a' + 10 : ((ch >= 'A') ? ch - 'A' + 10 : evalDigit(ch));
	}



	/**
	 * @brief Present a character in a human-readable form.
	*/
	std::string escape(char ch);



	/**
	 * @brief Convert a character to lowercase.
	*/
	inline char toLower(char ch) noexcept {
		return static_cast<char>(std::tolower(toInt(ch)));
	}



	/**
	 * @brief Convert a character to uppercase.
	*/
	inline char toUpper(char ch) noexcept {
		return static_cast<char>(std::toupper(toInt(ch)));
	}



	/**
	 * @brief Check if a character is lowercase.
	*/
	inline bool isLower(char ch) noexcept {
		return std::islower(toInt(ch));
	}



	/**
	 * @brief Check if a character is uppercase.
	*/
	inline bool isUpper(char ch) noexcept {
		return std::isupper(toInt(ch));
	}



	/**
	 * @brief Check if a character is an alphabetic character.
	*/
	inline bool isAlpha(char ch) noexcept {
		return std::isalpha(toInt(ch));
	}



	/**
	 * @brief Check if a character is a digit.
	*/
	inline bool isDigit(char ch) noexcept {
		return std::isdigit(toInt(ch));
	}



	/**
	 * @brief Check if a character is a hexadecimal digit.
	*/
	inline bool isHexDigit(char ch) noexcept {
		return std::isxdigit(toInt(ch));
	}



	/**
	 * @brief Check if a character is an alphanumeric character.
	*/
	inline bool isAlnum(char ch) noexcept {
		return std::isalnum(toInt(ch));
	}



	/**
	 * @brief Check if a character is a whitespace character.
	*/
	inline bool isSpace(char ch) noexcept {
		return std::isspace(toInt(ch));
	}



	/**
	 * @brief Check if a character is a printable character.
	*/
	inline bool isPrint(char ch) noexcept {
		return std::isprint(toInt(ch));
	}

}
