#include "fg/Symbol.hpp"

#include "fg/RuleConcat.hpp"
#include "fg/Atom.hpp"

namespace parsec::fg {
	Symbol::Symbol(const std::string& name, RulePtr rule, SymbolTypes type, int id)
		: m_name(name), m_type(type), m_id(id) {
		m_rule = makeRule<RuleConcat>(
			std::move(rule),
			makeRule<Atom>('$')
		);
	}
}