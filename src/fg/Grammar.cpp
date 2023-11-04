#include "fg/Grammar.hpp"

#include "fg/AtomVisitor.hpp"
#include "fg/RuleAltern.hpp"
#include "fg/NilRule.hpp"

#include <gsl/narrow>
#include <set>

namespace parsec::fg {
	namespace {
		class BuildStartSymbol : AtomVisitor {
		public:
			explicit BuildStartSymbol(const Grammar& grammar)
				: m_grammar(grammar)
			{ }

			Symbol operator()() {
				// any nonterminal symbol can be a root symbol
				for(const auto sym : m_grammar.nonterminals()) {
					m_rootSymbols.insert(sym->id());
				}

				// find all unreferenced symbols
				for(const auto sym : m_grammar.nonterminals()) {
					m_currentSymbol = sym;
					sym->ruleBody()->traverse(*this);
				}

				// combine found symbols into one
				RulePtr rule;
				for(const auto id : m_rootSymbols) {
					const auto sym = m_grammar.symbolById(id);
					if(auto lhs = std::exchange(rule, makeRule<Atom>(sym->name()))) {
						rule = makeRule<RuleAltern>(std::move(lhs), std::move(rule));
					}
				}
				
				// the start symbol is empty, if there are no root symbols
				if(!rule) {
					rule = makeRule<NilRule>();
				}

				// create a symbol of the form: start = (root1 | root2 | ... ) eof
				return Symbol("start",
					makeRule<RuleConcat>(
						std::move(rule), makeRule<Atom>("eof")
					),
					SymbolTypes::Start,
					0
				);
			}

		private:
			void visit(const Atom& n) override {
				const auto sym = m_grammar.symbolByName(n.value());
				if(sym && sym != m_currentSymbol) { // a root symbol can be a reference to itself
					m_rootSymbols.erase(sym->id());
				}
			}

			const Symbol* m_currentSymbol = nullptr;
			std::set<int> m_rootSymbols;
			const Grammar& m_grammar;
		};
	}


	Grammar::Grammar() {
		putSymbol("eof", makeRule<NilRule>(), SymbolTypes::End);
	}


	void Grammar::putSymbol(const std::string& name, RulePtr rule, SymbolTypes type) {
		// try to construct a new named symbol if it doesn't already exist
		const auto newSymbolId = gsl::narrow_cast<int>(m_symbols.size());
		const auto [it, wasInserted] = m_symbolTable.try_emplace(
			name, name, std::move(rule), type, newSymbolId
		);
		const auto& sym = it->second;

		// for now just ignore duplicate symbols
		if(!wasInserted) {
			return;
		}

		// classify the symbol for easier access later
		m_symbols.push_back(&sym);
		if(sym.isTerminal()) {
			m_terminals.push_back(&sym);
		} else if(sym.isNonterminal()) {
			m_nonterminals.push_back(&sym);
		}

		// reset the start symbol, as it might need to be recalculated due to the grammar modification
		m_startSymbol.reset();
	}


	const Symbol* Grammar::symbolByName(const std::string& name) const {
		const auto it = m_symbolTable.find(name);
		if(it != m_symbolTable.cend()) {
			return &it->second;
		}
		return nullptr;
	}

	const Symbol* Grammar::startSymbol() const {
		if(!m_startSymbol) {
			m_startSymbol = BuildStartSymbol(*this)();
		}
		return std::to_address(m_startSymbol);
	}
}