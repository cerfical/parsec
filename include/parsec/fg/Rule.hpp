#ifndef PARSEC_FG_RULE_HEADER
#define PARSEC_FG_RULE_HEADER

#include <ostream>
#include <memory>
#include <vector>

namespace parsec::fg {
	class RuleVisitor;
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
		virtual void acceptVisitor(RuleVisitor& visitor) const = 0;
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
		
		const Atom* endAtom() const noexcept;

		bool isNullable() const noexcept;
		/** @} */


	private:
		const Rule* m_parent;
	};

	using RulePtr = std::unique_ptr<Rule>;


	template <typename Rule, typename... Args>
		requires std::constructible_from<Rule, Args...>
	RulePtr makeRule(Args&&... args) {
		return std::make_unique<Rule>(std::forward<Args>(args)...);
	}
}

#endif