#ifndef PARSEC_CHAR_ATOM_HEADER
#define PARSEC_CHAR_ATOM_HEADER

#include <vector>
#include <ranges>

namespace parsec {
	/**
	 * @brief Represents characters that form some @ref Pattern "pattern".
	 */
	class CharAtom {
	public:
		CharAtom(char value, int id) noexcept
			: m_id(id), m_value(value)
		{ }

		
		/** @{ */
		CharAtom(const CharAtom&) = delete;
		CharAtom& operator=(const CharAtom&) = delete;
		/** @} */


		/** @{ */
		/** @brief List of character atoms that can follow this atom. */
		std::ranges::view auto follows() const {
			return std::ranges::ref_view(m_follows);
		}

		/** @brief Add an atom to the follows-list of this atom. */
		void addFollow(const CharAtom* atom) {
			m_follows.push_back(atom);
		}
		/** @} */


		/** @{ */
		char value() const noexcept {
			return m_value;
		}

		int id() const noexcept {
			return m_id;
		}
		/** @} */


	private:
		std::vector<const CharAtom*> m_follows;

		int m_id;
		char m_value;
	};
}

#endif