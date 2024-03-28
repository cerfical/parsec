#ifndef PARSEC_ELR_STATE_ITEM_HEADER
#define PARSEC_ELR_STATE_ITEM_HEADER

#include <compare>

namespace parsec::elr {

	class TransNetwork;

	class StateItem {
	public:

		friend auto operator<=>(const StateItem&, const StateItem&) = default;


		StateItem(int dfaState, int backLink = -1)
			: m_dfaState(dfaState), m_backLink(backLink) {}


		int dfaState() const noexcept {
			return m_dfaState;
		}

		int backLink() const {
			return m_backLink;
		}


	private:
		int m_backLink = {};
		int m_dfaState = {};
	};

}

#endif