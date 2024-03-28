#ifndef PARSEC_ELR_REDUCE_ACTION_HEADER
#define PARSEC_ELR_REDUCE_ACTION_HEADER

#include "../fg/Symbol.hpp"

namespace parsec::elr {

	class ReduceAction {
	public:

		ReduceAction() = default;

		ReduceAction(const fg::Symbol& reduceRule, int backLink)
			: m_reduceRule(reduceRule), m_backLink(backLink) {}


		const fg::Symbol& reduceRule() const {
			return m_reduceRule;
		}

		int backLink() const {
			return m_backLink;
		}


	private:
		fg::Symbol m_reduceRule;
		int m_backLink = {};
	};

}

#endif