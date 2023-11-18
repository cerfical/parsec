#ifndef PARSEC_PATTERN_HEADER
#define PARSEC_PATTERN_HEADER

#include "CharAtom.hpp"

#include <ranges>
#include <vector>
#include <list>

namespace parsec {
	class Symbol;

	class Pattern {
	public:
		/** @{ */
		Pattern(const Symbol* head, int id)
			: m_head(head), m_id(id) {
			addAtom('$');
		}
		/** @} */


		/** @{ */
		Pattern(Pattern&&) = default;
		Pattern& operator=(Pattern&&) = default;
		/** @} */

		/** @{ */
		Pattern(const Pattern&) = delete;
		Pattern& operator=(const Pattern&) = delete;
		/** @} */


		/** @{ */
		const Symbol* head() const noexcept {
			return m_head;
		}

		const CharAtom* endAtom() const noexcept {
			return &m_atoms.front();
		}

		int id() const noexcept {
			return m_id;
		}
		/** @} */


		/** @{ */
		std::ranges::view auto atoms() const {
			return m_atoms | std::views::transform(
				[](const auto& a) { return &a; }
			);
		}

		CharAtom* addAtom(char value);

		CharAtom* addRoot(char value) {
			auto a = addAtom(value);
			markRoot(a);
			return a;
		}
		/** @} */


		/** @{ */
		std::ranges::view auto roots() const {
			return std::ranges::ref_view(m_roots);
		}

		void markRoot(const CharAtom* atom) {
			m_roots.push_back(atom);
		}
		/** @} */


	private:
		std::list<CharAtom> m_atoms;
		std::vector<const CharAtom*> m_roots;

		const Symbol* m_head;
		int m_id;
	};
}

#endif