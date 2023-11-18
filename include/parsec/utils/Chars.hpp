#ifndef PARSEC_UTILS_CHARS_HEADER
#define PARSEC_UTILS_CHARS_HEADER

#include <string>
#include <cctype>

namespace parsec {
	class Chars {
	public:
		/** @{ */
		/** @brief Safely convert a character to an integer. */
		static constexpr int charToInt(char ch) noexcept {
			return static_cast<unsigned char>(ch);
		}

		/** @brief Find the value of a single digit represented as a character. */
		static constexpr int evalDigit(char ch) noexcept {
			return ch - '0';
		}

		/** @brief Find the value of a single hexadecimal digit represented as a character. */
		static constexpr int evalHexDigit(char ch) noexcept {
			return (ch >= 'a') ? ch - 'a' + 10 : ((ch >= 'A') ? ch - 'A' : evalDigit(ch));
		}
		/** @} */


		/** @{ */
		/** @brief Present a character in a human-readable form. */
		static std::string escapeChar(char ch);

		/** @brief Convert a character to uppercase. */
		static char toUpper(char ch) {
			return static_cast<char>(std::toupper(charToInt(ch)));
		}

		/** @brief Convert a character to lowercase. */
		static char toLower(char ch) {
			return static_cast<char>(std::tolower(charToInt(ch)));
		}
		/** @} */


		/** @{ */
		/** @brief Checks whether a character is in lowercase. */
		static bool isLower(char ch) noexcept {
			return std::islower(charToInt(ch));
		}

		/** @brief Checks whether a character is in uppercase. */
		static bool isUpper(char ch) noexcept {
			return std::isupper(charToInt(ch));
		}

		/** @brief Checks if a character is an alphabetic character. */
		static bool isAlpha(char ch) noexcept {
			return std::isalpha(charToInt(ch));
		}


		/** @brief Checks if a character is a digit. */
		static bool isDigit(char ch) noexcept {
			return std::isdigit(charToInt(ch));
		}

		/** @brief Checks if a character is a hexadecimal digit. */
		static bool isHexDigit(char ch) noexcept {
			return std::isxdigit(charToInt(ch));
		}

		/** @brief Checks if a character is an alphanumeric character. */
		static bool isAlnum(char ch) noexcept {
			return std::isalnum(charToInt(ch));
		}


		/** @brief Checks if a character is a whitespace character. */
		static bool isSpace(char ch) noexcept {
			return std::isspace(charToInt(ch));
		}

		/** @brief Checks if a character is a printable character. */
		static bool isPrint(char ch) noexcept {
			return std::isprint(charToInt(ch));
		}
		/** @} */


	private:
		Chars() = delete;
		~Chars() = delete;
	};
}

#endif