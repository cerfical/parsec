#pragma once

#include "RegularExpr.hpp"
#include "Symbol.hpp"

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

}
