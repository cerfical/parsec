#pragma once

#include "Symbol.hpp"
#include "Error.hpp"

namespace parsec {

	class RuleConflictError : public Error {
	public:

		RuleConflictError(const Symbol& rule1, const Symbol& rule2)
			: Error("rule conflict error"), m_rule1(rule1), m_rule2(rule2) {}


		const Symbol& rule1() const {
			return m_rule1;
		}
		
		const Symbol& rule2() const {
			return m_rule2;
		}


	private:
		Symbol m_rule1;
		Symbol m_rule2;
	};

}
