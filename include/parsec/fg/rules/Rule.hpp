#ifndef PARSEC_FG_RULES_RULE_HEADER
#define PARSEC_FG_RULES_RULE_HEADER

#include <ostream>
#include <vector>

namespace parsec::fg::rules {
	class RuleTraverser;
	class Atom;

	using AtomList = std::vector<const Atom*>;


	class Rule {
	public:
		/** @{ */
		friend std::ostream& operator<<(std::ostream& out, const Rule& n);
		/** @} */


		/** @{ */
		explicit Rule(const Rule* parent = nullptr) noexcept
			: m_parent(parent)
		{ }

		virtual ~Rule() = default;
		/** @} */


		/** @{ */
		Rule(const Rule&) = delete;
		Rule& operator=(const Rule&) = delete;
		/** @} */


		/** @{ */
		Rule(Rule&&) = delete;
		Rule& operator=(Rule&&) = delete;
		/** @} */


		/** @{ */
		virtual void traverse(RuleTraverser& traverser) const = 0;
		/** @} */


		/** @{ */
		void setParent(const Rule* parent) noexcept {
			m_parent = parent;
		}

		const Rule* parent() const noexcept {
			return m_parent;
		}
		/** @} */


		/** @{ */
		AtomList leadingAtoms() const;
		AtomList trailingAtoms() const;

		bool nullable() const noexcept;
		/** @} */


	private:
		/** @{ */
		const Rule* m_parent;
		/** @} */
	};
}

#endif