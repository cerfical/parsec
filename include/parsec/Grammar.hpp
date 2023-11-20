#ifndef PARSEC_GRAMMAR_HEADER
#define PARSEC_GRAMMAR_HEADER

#include "Symbol.hpp"

#include <unordered_map>
#include <ranges>
#include <vector>
#include <list>

namespace parsec {
	class Grammar {
	public:
		/** @{ */
		Grammar();
		/** @} */


		/** @{ */
		Grammar(Grammar&&) = default;
		Grammar& operator=(Grammar&&) = default;
		/** @} */

		/** @{ */
		Grammar(const Grammar&) = delete;
		Grammar& operator=(const Grammar&) = delete;
		/** @} */


		/** @{ */
		void replaceSymbol(const Symbol* symbol, const Symbol* replace) {
			for(auto& rule : m_rules) {
				rule.replaceSymbol(symbol, replace);
			}
		}


		const Symbol* symbolForName(const std::string& name) const;

		const Symbol* symbolForId(int id) const noexcept {
			return m_sortedSymbols[id];
		}


		std::ranges::view auto symbols() const {
			return m_sortedSymbols | std::views::transform(
				[](auto r) { return static_cast<const Symbol*>(r); }
			);
		}
		/** @} */


		/** @{ */
		Pattern* insertPattern(const Symbol* head);
		
		Pattern* insertPattern(const std::string& name) {
			return insertPattern(symbolForName(name));
		}


		std::ranges::view auto tokenSymbols() const {
			return symbols() | std::views::filter(
				[this](const auto s) { return s->pattern() || s == eof() || s == ws(); }
			);
		}
		
		std::ranges::view auto tokenPatterns() const {
			return m_tokenPatterns | std::views::transform(
				[](const auto& t) { return &t; }
			);
		}

		const Symbol* eof() const noexcept {
			return m_sortedSymbols[1];
		}

		const Symbol* ws() const noexcept {
			return m_sortedSymbols[2];
		}
		/** @} */


		/** @{ */
		Rule* insertRule(const Symbol* head);

		Rule* insertRule(const std::string& name) {
			return insertRule(symbolForName(name));
		}

		
		std::ranges::view auto ruleSymbols() const {
			return symbols() | std::views::filter(
				[this](const auto s) { return !s->rules().empty() || s == root(); }
			);
		}

		std::ranges::view auto rules() const {
			return m_rules | std::views::transform(
				[](const auto& r) { return &r; }
			);
		}


		const Symbol* root() const noexcept {
			return m_sortedSymbols[0];
		}
		/** @} */


	private:
		// use list to avoid dangling pointers to stored objects
		std::list<Pattern> m_tokenPatterns;
		std::list<Rule> m_rules;
		
		// use vector to cache sorted by id symbols
		mutable std::unordered_map<std::string, Symbol> m_symbols;
		mutable std::vector<Symbol*> m_sortedSymbols;
	};
}

#endif