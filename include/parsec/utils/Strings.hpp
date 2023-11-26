#ifndef PARSEC_UTILS_STRINGS_HEADER
#define PARSEC_UTILS_STRINGS_HEADER

#include <string_view>
#include <string>

namespace parsec::utils {
	/**
	 * @brief Utilities for working with character strings.
	 */
	class Strings {
	public:
		static std::string capitalize(std::string_view s);

		static std::string toLower(std::string_view s);
		static std::string toUpper(std::string_view s);

	private:
		Strings() = delete;
		~Strings() = delete;
	};
}

#endif