#ifndef PARSEC_CORE_SYMBOL_HEADER
#define PARSEC_CORE_SYMBOL_HEADER

#include <string>
#include <string_view>

namespace parsec {

	class Symbol {
	public:

		friend auto operator<=>(const Symbol&, const Symbol&) = default;


		Symbol(char value)
			: m_value(1, value) {}

		Symbol(const char* value)
			: m_value(value) {}

		Symbol(std::string_view value)
			: m_value(value) {}

		Symbol(const std::string& value)
			: m_value(value) {}

		Symbol(std::string&& value)
			: m_value(std::move(value)) {}

		Symbol() = default;


		explicit operator bool() const {
			return !isEmpty();
		}

		bool isEmpty() const {
			return value().empty();
		}
		
		const std::string& value() const {
			return m_value;
		}


	private:
		std::string m_value;
	};

}

#endif