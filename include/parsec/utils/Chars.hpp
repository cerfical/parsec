#ifndef PARSEC_UTILS_CHARS_HEADER
#define PARSEC_UTILS_CHARS_HEADER

#include <string>
#include <cctype>

namespace parsec::utils {
	/**
	 * @brief Utilities for working with characters.
	 */
	class Chars {
	public:
		/** @brief Safely convert a character to an integer. */
		static constexpr int toInt(char ch) noexcept {
			return static_cast<unsigned char>(ch);
		}


		/** @brief Find the value of a digit character. */
		static constexpr int evalDigit(char ch) noexcept {
			return ch - '0';
		}

		/** @brief Find the value of a hexadecimal digit character. */
		static constexpr int evalHexDigit(char ch) noexcept {
			return (ch >= 'a') ? ch - 'a' + 10 : ((ch >= 'A') ? ch - 'A' : evalDigit(ch));
		}


		/** @brief Present a character in a human-readable form. */
		static std::string escape(char ch);


		/** @{ */
		static char toLower(char ch) noexcept {
			return static_cast<char>(std::tolower(toInt(ch)));
		}
		
		static char toUpper(char ch) noexcept {
			return static_cast<char>(std::toupper(toInt(ch)));
		}
		/** @} */


		/** @{ */
		static bool isLower(char ch) noexcept {
			return std::islower(toInt(ch));
		}

		static bool isUpper(char ch) noexcept {
			return std::isupper(toInt(ch));
		}


		static bool isAlpha(char ch) noexcept {
			return std::isalpha(toInt(ch));
		}

		static bool isDigit(char ch) noexcept {
			return std::isdigit(toInt(ch));
		}

		static bool isHexDigit(char ch) noexcept {
			return std::isxdigit(toInt(ch));
		}

		static bool isAlnum(char ch) noexcept {
			return std::isalnum(toInt(ch));
		}


		static bool isSpace(char ch) noexcept {
			return std::isspace(toInt(ch));
		}

		static bool isPrint(char ch) noexcept {
			return std::isprint(toInt(ch));
		}
		/** @} */


	private:
		Chars() = delete;
		~Chars() = delete;
	};
}

#endif