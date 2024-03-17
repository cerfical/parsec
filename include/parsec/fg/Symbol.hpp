#ifndef PARSEC_FG_SYMBOL_HEADER
#define PARSEC_FG_SYMBOL_HEADER

#include <string>
#include <ostream>

namespace parsec::fg {

	class Symbol {
	public:

		Symbol(char ch)
			: Symbol(std::string(1, ch)) {}

		Symbol(const std::string& name)
			: m_name(name) {}

		Symbol() = default;


		const std::string& name() const {
			return m_name;
		}

		bool isEmpty() const {
			return m_name.empty();
		}


		explicit operator bool() const {
			return !isEmpty();
		}


	private:
		std::string m_name;
	};


	inline std::ostream& operator<<(std::ostream& out, const Symbol& symbol) {
		return out << symbol.name();
	}

}

#endif