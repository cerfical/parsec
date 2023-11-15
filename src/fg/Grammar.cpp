#include "fg/Grammar.hpp"

#include "fg/RuleTraverser.hpp"
#include "fg/RuleAltern.hpp"
#include "fg/RuleConcat.hpp"
#include "fg/NilRule.hpp"
#include "fg/Atom.hpp"

#include <format>
#include <ranges>

namespace views = std::ranges::views;

namespace parsec::fg {
	namespace {
		constexpr auto wsName = "Ws";
		constexpr auto eofName = "Eof";
		constexpr auto startName = "_Start";
		constexpr auto endMark = "$";

		RulePtr appendEndAtom(RulePtr rule, const std::string& value = endMark) {
			return makeRule<RuleConcat>(
				std::move(rule),
				makeRule<Atom>(value)
			);
		}

		class BuildStartRule : RuleTraverser {
		public:
			explicit BuildStartRule(const Grammar& grammar) noexcept
				: m_grammar(grammar)
			{ }

			RulePtr operator()() {
				// any nonterminal symbol can be a root symbol
				m_rootSymbols.resize(
					m_grammar.ruleCount(),
					true
				);

				// find all unreferenced symbols
				for(const auto sym : m_grammar.rules()) {
					m_symbolEndAtom = sym->rule()->endAtom();
					m_ruleSymbol = sym;

					traverse(*sym->rule());
				}

				// combine found symbols into one
				RulePtr rule;
				for(const auto i : views::iota(std::size_t(0), m_rootSymbols.size())) {
					if(!m_rootSymbols[i]) {
						continue;
					}

					const auto sym = m_grammar.rules()[i];
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
				if(&n == m_symbolEndAtom) {
					return;
				}

				// skip references to undefined symbols
				const auto sym = m_grammar.symbolByName(n.value());
				if(!sym) {
					return;
				}
				
				// a root symbol can have a reference to itself
				if(sym == m_ruleSymbol) {
					return;
				}

				// but cannot be referenced by other symbols
				if(sym->definesRule()) {
					m_rootSymbols[sym->id()] = false;
				}
			}


			const Atom* m_symbolEndAtom = nullptr;
			const Symbol* m_ruleSymbol = 0;

			std::vector<bool> m_rootSymbols;
			const Grammar& m_grammar;
		};
	}


	Grammar::Grammar() {
		auto& start = m_symbolTable[startName] = Symbol(startName, SymbolTypes::Rule);
		auto& eof = m_symbolTable[eofName] = Symbol(eofName, SymbolTypes::Token);
		auto& ws = m_symbolTable[wsName] = Symbol(wsName, SymbolTypes::Token);
		
		m_symbols.push_back(&start);
		m_symbols.push_back(&eof);
		m_symbols.push_back(&ws);
	}


	void Grammar::initNewSymbol(Symbol& sym, RulePtr rule) {
		sym.setRule(appendEndAtom(std::move(rule)));

		if(sym.definesToken()) {
			sym.setId(tokenCount());
			m_tokens.push_back(&sym);
		} else {
			sym.setId(ruleCount());
			m_rules.push_back(&sym);
		}
		
		m_symbols.push_back(&sym);
	}

	void Grammar::appendRuleToSymbol(Symbol& sym, RulePtr rule) const {
		if(!sym.hasRule()) {
			// if there is no rule attached to the symbol, attach one
			sym.setRule(appendEndAtom(std::move(rule)));
		} else {
			// otherwise combine two alternative rules into one
			const auto symRule = static_cast<RuleConcat*>(sym.rule());
			symRule->setLeft(
				makeRule<RuleAltern>(
					symRule->takeLeft(),
					std::move(rule)
				)
			);
		}
	}

	Symbol* Grammar::insertSymbol(const std::string& name, RulePtr rule, SymbolTypes type) {
		// allocate a new symbol if it doesn't already exist
		const auto [it, wasInserted] = m_symbolTable.try_emplace(name, name, type);
		auto& sym = it->second;

		if(wasInserted) {
			initNewSymbol(sym, std::move(rule));
		} else if(sym.type() == type) {
			appendRuleToSymbol(sym, std::move(rule));
		} else {
			return nullptr;
		}

		return &sym;
	}


	const Symbol* Grammar::symbolByName(const std::string& name) const {
		const auto it = m_symbolTable.find(name);
		if(it != m_symbolTable.cend()) {
			return &it->second;
		}
		return nullptr;
	}

	const Symbol* Grammar::startSymbol() const {
		const auto startSymbol = m_symbols[0];
		if(!startSymbol->hasRule()) {
			// create a symbol of the form: _Start = (root1 | root2 | ... ) Eof $
			const_cast<Symbol*>(startSymbol)->setRule(
				appendEndAtom(
					appendEndAtom(
						BuildStartRule(*this)(),
						eofName
					)
				)
			);
		}
		return startSymbol;
	}
}