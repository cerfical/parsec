#ifndef PARSEC_FG_ATOM_HEADER
#define PARSEC_FG_ATOM_HEADER

#include "Rule.hpp"
#include <string>

namespace parsec::fg {
	class Atom : public Rule {
	public:
		/** @{ */
		explicit Atom(char ch)
			: m_value(1, ch)
		{ }

		explicit Atom(const std::string& value)
			: m_value(value)
		{ }

		~Atom() override = default;
		/** @} */


		/** @{ */
		void traverse(RuleTraverser& traverser) const;
		/** @} */


		/** @{ */
		const std::string& value() const noexcept {
			return m_value;
		}
		
		AtomList nextAtoms() const;
		/** @} */


	private:
		/** @{ */
		std::string m_value;
		/** @} */
	};
}

#endif