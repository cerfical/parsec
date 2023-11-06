#include "fg/Grammar.hpp"

#include <boost/algorithm/string/case_conv.hpp>

#include "fg/AtomVisitor.hpp"
#include "fg/RuleAltern.hpp"
#include "fg/RuleConcat.hpp"
#include "fg/NilRule.hpp"
#include "fg/Atom.hpp"

#include <gsl/narrow>
#include <set>

namespace parsec::fg {
	namespace {
		RulePtr appendEndAtom(RulePtr rule, const std::string& value) {
			return makeRule<RuleConcat>(
				std::move(rule),
				makeRule<Atom>(value)
			);
		}

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
					m_endAtom = sym->rule()->endAtom();
					sym->rule()->traverse(*this);
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

				// create a symbol of the form: start = (root1 | root2 | ... ) eof $
				return Symbol("start",
					appendEndAtom(
						appendEndAtom(std::move(rule), "eof"),
						"$"
					),
					SymbolTypes::Start, 0
				);
			}

		private:
			void visit(const Atom& n) override {
				// skip end atoms as a special case
				if(&n == m_endAtom) {
					return;
				}

				const auto sym = m_grammar.symbolByName(n.value());
				if(sym && sym != m_currentSymbol) { // a root symbol can be a reference to itself
					m_rootSymbols.erase(sym->id());
				}
			}

			const Symbol* m_currentSymbol = nullptr;
			const Atom* m_endAtom = nullptr;
			std::set<int> m_rootSymbols;
			const Grammar& m_grammar;
		};
	}


	Grammar::Grammar() {
		addSymbol("eof", makeRule<NilRule>(), SymbolTypes::End);
	}

	Symbol* Grammar::addSymbol(const std::string& name, RulePtr rule, SymbolTypes type) {
		// enforce symbol names to be in lower case
		const auto symbolName = boost::to_lower_copy(name);

		// allocate a new defaulted symbol if it doesn't already exist
		const auto [it, wasInserted] = m_symbolTable.try_emplace(symbolName);
		auto& sym = it->second;

		if(wasInserted) {
			const auto newSymbolId = gsl::narrow_cast<int>(m_symbols.size());
			sym = Symbol(
				symbolName,
				appendEndAtom(std::move(rule), "$"),
				type,
				newSymbolId
			);

			// classify the symbol for easier access later
			m_symbols.push_back(&sym);
			if(sym.isTerminal()) {
				m_terminals.push_back(&sym);
			} else if(sym.isNonterminal()) {
				m_nonterminals.push_back(&sym);
			}
		} else {
			if(sym.type() != type) {
				return nullptr;
			}

			// if the symbol is not new, add the rule to its definition
			auto& lhs = static_cast<RuleConcat*>(
				const_cast<Rule*>(sym.rule())
			)->left();

			lhs = makeRule<RuleAltern>(
				std::move(lhs),
				std::move(rule)
			);
		}

		// reset the start symbol, as it might need to be recalculated due to the grammar modification
		m_startSymbol.reset();
		return &sym;
	}

	const Symbol* Grammar::symbolByName(const std::string& name) const {
		const auto symbolName = boost::to_lower_copy(name);
		const auto it = m_symbolTable.find(symbolName);
		
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