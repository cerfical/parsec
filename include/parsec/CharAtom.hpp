#ifndef PARSEC_CHAR_ATOM_HEADER
#define PARSEC_CHAR_ATOM_HEADER

#include <vector>
#include <ranges>

namespace parsec {
	class CharAtom {
	public:
		/** @{ */
		CharAtom(char value, int id) noexcept
			: m_id(id), m_value(value)
		{ }
		/** @} */

		
		/** @{ */
		CharAtom(CharAtom&&) = default;
		CharAtom& operator=(CharAtom&&) = default;
		/** @} */
		
		/** @{ */
		CharAtom(const CharAtom&) = delete;
		CharAtom& operator=(const CharAtom&) = delete;
		/** @} */


		/** @{ */
		char value() const noexcept {
			return m_value;
		}

		int id() const noexcept {
			return m_id;
		}
		/** @} */


		/** @{ */
		std::ranges::view auto follows() const {
			return std::ranges::ref_view(m_follows);
		}

		void addFollow(const CharAtom* atom) {
			m_follows.push_back(atom);
		}
		/** @} */


	private:
		std::vector<const CharAtom*> m_follows;

		int m_id;
		char m_value;
	};
}

#endif