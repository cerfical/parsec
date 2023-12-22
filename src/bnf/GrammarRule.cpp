#include "bnf/GrammarSymbol.hpp"
#include <algorithm>

namespace parsec::bnf {
	std::size_t GrammarRule::nonTerminalCount() const noexcept {
		return std::ranges::count_if(m_body, [](auto s) { return s->isNonTerminal(); });
	}

	std::size_t GrammarRule::terminalCount() const noexcept {
		return std::ranges::count_if(m_body, [](auto s) { return s->isTerminal(); });
	}

	std::string_view GrammarRule::name() const noexcept {
		return head()->name();
	}
}