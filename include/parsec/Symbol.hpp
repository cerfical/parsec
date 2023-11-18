#ifndef PARSEC_SYMBOL_HEADER
#define PARSEC_SYMBOL_HEADER

#include "Rule.hpp"
#include "Pattern.hpp"

#include <string>
#include <vector>
#include <ranges>

namespace parsec {
	class Symbol {
	public:
		/** @{ */
		Symbol(const std::string& name, int id)
			: m_name(name), m_id(id)
		{ }
		/** @} */


		/** @{ */
		Symbol(Symbol&&) = default;
		Symbol& operator=(Symbol&&) = default;
		/** @} */

		/** @{ */
		Symbol(const Symbol&) = delete;
		Symbol& operator=(const Symbol&) = delete;
		/** @} */


		/** @{ */
		std::ranges::view auto rules() const {
			return m_rules | std::views::transform(
				[](auto r) { return static_cast<const Rule*>(r); }
			);
		}
		
		std::ranges::view auto rules() {
			return std::ranges::ref_view(m_rules);
		}
		
		void addRule(Rule* rule) {
			m_rules.push_back(rule);
		}
		/** @} */


		/** @{ */
		void setPattern(Pattern* pattern) {
			m_pattern = pattern;
		}
		

		const Pattern* pattern() const noexcept {
			return m_pattern;
		}

		Pattern* pattern() noexcept {
			return m_pattern;
		}
		/** @} */


		/** @{ */
		const std::string& name() const noexcept {
			return m_name;
		}

		int id() const noexcept {
			return m_id;
		}
		/** @} */


	private:
		std::vector<Rule*> m_rules;
		Pattern* m_pattern = nullptr;

		std::string m_name;
		int m_id;
	};
}

#endif