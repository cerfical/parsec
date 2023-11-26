#ifndef PARSEC_PATTERN_HEADER
#define PARSEC_PATTERN_HEADER

#include "CharAtom.hpp"

#include <ranges>
#include <vector>
#include <list>

namespace parsec {
	class Symbol;

	/**
	 * @brief Compiled regex pattern.
	 */
	class Pattern {
	public:
		Pattern(const Symbol* head, int id)
			: m_head(head), m_id(id) {
			addAtom('$');
		}


		/** @{ */
		Pattern(const Pattern&) = delete;
		Pattern& operator=(const Pattern&) = delete;
		/** @} */


		/** @{ */
		/** @brief A special atom indicating the end of the pattern. */
		const CharAtom* endAtom() const noexcept {
			return &m_atoms.front();
		}
		
		const Symbol* head() const noexcept {
			return m_head;
		}

		int id() const noexcept {
			return m_id;
		}
		/** @} */


		/** @{ */
		CharAtom* addAtom(char value);

		CharAtom* addRoot(char value) {
			auto a = addAtom(value);
			markRoot(a);
			return a;
		}
		
		std::ranges::view auto atoms() const {
			return m_atoms | std::views::transform(
				[](const auto& a) { return &a; }
			);
		}
		/** @} */


		/** @{ */
		void markRoot(const CharAtom* atom) {
			m_roots.push_back(atom);
		}

		std::ranges::view auto roots() const {
			return std::ranges::ref_view(m_roots);
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