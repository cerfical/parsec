#ifndef PARSEC_ELR_REDUCE_ACTION_HEADER
#define PARSEC_ELR_REDUCE_ACTION_HEADER

#include "../core/Symbol.hpp"

namespace parsec::elr {

	class ReduceAction {
	public:

		ReduceAction() = default;

		ReduceAction(const Symbol& reduceRule, int backLink)
			: m_reduceRule(reduceRule), m_backLink(backLink) {}


		const Symbol& reduceRule() const {
			return m_reduceRule;
		}

		int backLink() const {
			return m_backLink;
		}


	private:
		Symbol m_reduceRule;
		int m_backLink = {};
	};

}

#endif