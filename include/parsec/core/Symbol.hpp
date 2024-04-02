#ifndef PARSEC_CORE_SYMBOL_HEADER
#define PARSEC_CORE_SYMBOL_HEADER

#include <string>
#include <string_view>
#include <ostream>

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


	inline std::ostream& operator<<(std::ostream& out, const Symbol& symbol) {
		return out << '\'' << symbol.value() << '\'';
	}

}

namespace std {

	template <>
	struct hash<parsec::Symbol> {
		size_t operator()(const parsec::Symbol& symbol) const noexcept {
			return hash<string>()(symbol.value());
		}
	};

}

#endif