#ifndef PARSEC_ELR_STATE_TRANS_HEADER
#define PARSEC_ELR_STATE_TRANS_HEADER

#include "../core/Symbol.hpp"

namespace parsec::elr {

	class StateTrans {
	public:

		StateTrans(int target, const Symbol& label) noexcept
			: m_label(label), m_target(target) {}


		const Symbol& label() const noexcept {
			return m_label;
		}


		int target() const noexcept {
			return m_target;
		}


	private:
		Symbol m_label;
		int m_target = {};
	};

}

#endif