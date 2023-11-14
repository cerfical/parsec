#include "fg/Grammar.hpp"

#include "fg/RuleTraverser.hpp"
#include "fg/RuleAltern.hpp"
#include "fg/RuleConcat.hpp"
#include "fg/NilRule.hpp"
#include "fg/Atom.hpp"

#include <boost/algorithm/string/case_conv.hpp>

#include <gsl/narrow>
#include <ranges>

namespace views = std::ranges::views;

namespace parsec::fg {
	namespace {
		RulePtr appendEndAtom(RulePtr rule, const std::string& value) {
			return makeRule<RuleConcat>(
				std::move(rule),
				makeRule<Atom>(value)
			);
		}


		class BuildStartRule : RuleTraverser {
		public:
			explicit BuildStartRule(const Grammar& grammar)
				: m_grammar(grammar)
			{ }

			RulePtr operator()() {
				// any nonterminal symbol can be a root symbol
				m_rootSymbols.resize(
					m_grammar.nonterminals().size(),
					true
				);

				// find all unreferenced symbols
				for(const auto sym : m_grammar.nonterminals()) {
					m_currentSymbol = sym;
					m_endAtom = sym->rule()->endAtom();

					traverse(*sym->rule());
				}

				// combine found symbols into one
				RulePtr rule;
				for(const auto i : views::iota(0) | views::take(m_rootSymbols.size())) {
					if(!m_rootSymbols[i]) {
						continue;
					}

					const auto sym = m_grammar.nonterminals()[i];
					if(auto lhs = std::exchange(rule, makeRule<Atom>(sym->name()))) {
						rule = makeRule<RuleAltern>(
							std::move(lhs),
							std::move(rule)
						);
					}
				}
				
				// the start symbol is empty, if there are no root symbols
				if(!rule) {
					rule = makeRule<NilRule>();
				}

				return rule;
			}

		private:
			void visit(const Atom& n) override {
				// skip end atoms as a special case
				if(&n == m_endAtom) {
					return;
				}

				// skip references to undefined symbols
				const auto sym = m_grammar.symbolByName(n.value());
				if(!sym) {
					return;
				}
				
				// a root symbol can have a reference to itself
				if(sym == m_currentSymbol) {
					return;
				}

				// but cannot have references to other symbols
				if(sym->isNonterminal()) {
					m_rootSymbols[sym->id()] = false;
				}
			}

			const Symbol* m_currentSymbol = nullptr;
			const Atom* m_endAtom = nullptr;

			std::vector<bool> m_rootSymbols;
			const Grammar& m_grammar;
		};
	}


	Grammar::Grammar() {
		m_symbolTable.emplace(std::piecewise_construct,
			std::forward_as_tuple(eofSymbolName),
			std::forward_as_tuple(eofSymbolName, SymbolTypes::End)
		);
		
		m_symbolTable.emplace(std::piecewise_construct,
			std::forward_as_tuple(wsSymbolName),
			std::forward_as_tuple(wsSymbolName, SymbolTypes::Ws)
		);
	}


	Symbol* Grammar::addSymbol(const std::string& name, RulePtr rule, SymbolTypes type) {
		// enforce symbol names to be in lower case
		const auto symName = boost::to_lower_copy(name);

		// allocate a new symbol if it doesn't already exist
		const auto [it, wasInserted] = m_symbolTable.try_emplace(symName, symName);
		auto& sym = it->second;
		
		if(wasInserted) {
			// terminals and nonterminals have separate namespaces of unique ids
			sym.setId(gsl::narrow_cast<int>(
				type == SymbolTypes::Terminal ? m_terminals.size() : m_nonterminals.size()
			));

			sym.setRule(appendEndAtom(std::move(rule), ruleEndMark));
			sym.setType(type);

			// classify the symbol for easier access later
			if(sym.isTerminal()) {
				m_terminals.push_back(&sym);
			} else if(sym.isNonterminal()) {
				m_nonterminals.push_back(&sym);
			}
		} else {
			if(sym.isWs()) {
				// cannot combine nonterminal and ws symbols
				if(type == SymbolTypes::Nonterminal) {
					return nullptr;
				}

				// if the rule for the ws symbol was not yet defined, define one
				if(!sym.rule()) {
					sym.setRule(appendEndAtom(std::move(rule), ruleEndMark));
					return &sym;
				}
			} else if(sym.type() != type) {
				// if the symbol types don't match, fail to insert the symbol
				return nullptr;
			}

			// if the symbol is not new, add the rule to its definition
			auto& lhs = static_cast<RuleConcat*>(
				sym.rule().get()
			)->left();

			lhs = makeRule<RuleAltern>(
				std::move(lhs),
				std::move(rule)
			);
		}

		// reset the start symbol, as it might need to be recalculated due to the grammar modification
		if(type == SymbolTypes::Nonterminal) {
			m_startSymbol.reset();
		}
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
			// create a symbol of the form: start = (root1 | root2 | ... ) eof $
			m_startSymbol.emplace(
				"start", SymbolTypes::Start,
				appendEndAtom(
					appendEndAtom(
						BuildStartRule(*this)(),
						eofSymbolName
					),
					ruleEndMark
				)
			);
		}
		return std::to_address(m_startSymbol);
	}
}