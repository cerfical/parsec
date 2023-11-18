#ifndef PARSEC_UTILS_STRINGS_HEADER
#define PARSEC_UTILS_STRINGS_HEADER

#include <string>
#include <sstream>

namespace parsec {
	class Strings {
	public:
		/** @{ */
		template <typename T>
		static std::string toString(T&& value) {
			return (std::ostringstream()
				<< std::forward<T>(value)
			).str();
		}
		/** @} */


		/** @{ */
		static std::string capitalize(const std::string& str);

		static std::string toLower(const std::string& str);
		static std::string toUpper(const std::string& str);
		/** @} */


	private:
		Strings() = default;
		~Strings() = default;
	};
}

#endif