#ifndef PARSEC_CORE_SYMBOL_RULE_HEADER
#define PARSEC_CORE_SYMBOL_RULE_HEADER

#include "../print.hpp"
#include "RegularExpr.hpp"
#include "Symbol.hpp"

#include <ostream>

namespace parsec {

	class SymbolRule {
	public:

		SymbolRule() = default;

		SymbolRule(const Symbol& head, const RegularExpr& body)
			: m_head(head), m_body(body) {}


		explicit operator bool() const {
			return !isEmpty();
		}

		bool isEmpty() const {
			return body().isEmpty();
		}


		const Symbol& head() const {
			return m_head;
		}

		const RegularExpr& body() const {
			return m_body;
		}


	private:
		Symbol m_head;
		RegularExpr m_body;
	};


	inline std::ostream& operator<<(std::ostream& out, const SymbolRule& rule) {
		return out << rule.head() << ": " << rule.body();
	}

}

#endif