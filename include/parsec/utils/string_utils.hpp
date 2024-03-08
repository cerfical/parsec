#ifndef PARSEC_UTILS_STRING_UTILS_HEADER
#define PARSEC_UTILS_STRING_UTILS_HEADER

#include <string_view>
#include <string>

/**
 * @brief Utilities for working with character strings.
*/
namespace parsec::string_utils {

	/**
	 * @brief Make up a valid PascalCase identifier from a string.
	*/
	std::string toPascalCase(std::string_view str);



	/**
	 * @brief Make a first letter of a word uppercased.
	*/
	std::string capitalize(std::string_view str);



	/**
	 * @brief Convert all letters in a word to lowercase.
	*/
	std::string toLower(std::string_view str);



	/**
	 * @brief Convert all letters in a word to uppercase.
	*/
	std::string toUpper(std::string_view str);



	/**
	 * @brief Present a string in a human-readable form.
	*/
	std::string escape(std::string_view str);

}

#endif