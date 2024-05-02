#ifndef PARSEC_CORE_SYMBOL_HEADER
#define PARSEC_CORE_SYMBOL_HEADER

#include <compare>
#include <memory>
#include <string>
#include <string_view>

namespace parsec {

	class Symbol {
	public:

		Symbol() noexcept = default;

		Symbol(char value) : Symbol({}, 1, value) {}

		Symbol(const char* value) : Symbol({}, value) {}

		Symbol(std::string_view value) : Symbol({}, value) {}

		Symbol(const std::string& value) : Symbol({}, value) {}

		Symbol(std::string&& value) : Symbol({}, std::move(value)) {}


		Symbol(const Symbol& other) noexcept = default;
		Symbol& operator=(const Symbol& other) noexcept = default;


		Symbol(Symbol&& other) noexcept = default;
		Symbol& operator=(Symbol&& other) noexcept = default;


		const std::string& value() const noexcept {
			if(!m_value) {
				static const std::string empty;
				return empty;
			}
			return *m_value;
		}


		explicit operator bool() const noexcept {
			return !isEmpty();
		}


		bool isEmpty() const noexcept {
			return !m_value || m_value->empty();
		}
		

	private:
		template <typename... Args>
		explicit Symbol(int, Args&&... args)
			: m_value(std::make_shared<std::string>(std::forward<Args>(args)...)) {}

		std::shared_ptr<std::string> m_value;
	};



	inline bool operator==(const Symbol& lhs, const Symbol& rhs) noexcept {
		return lhs.value() == rhs.value();
	}


	inline std::strong_ordering operator<=>(const Symbol& lhs, const Symbol& rhs) noexcept {
		return lhs.value() <=> rhs.value();
	}

}

#endif