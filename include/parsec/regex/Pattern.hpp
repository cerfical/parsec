#ifndef PARSEC_REGEX_PATTERN_HEADER
#define PARSEC_REGEX_PATTERN_HEADER

#include "CharAtom.hpp"

#include <string_view>
#include <vector>

namespace parsec::regex {
	
	/**
	 * @brief Wraps a regular expression to provide some useful functionality.
	 */
	class Pattern {
	public:

		using AtomList = std::vector<const CharAtom*>;


		Pattern(std::string_view name, ExprPtr regex)
			: m_regex(std::move(regex)), m_name(name)
		{ }
		
		Pattern(std::string_view name, std::string_view regex)
			: Pattern(name, parseRegex(regex))
		{ }

		Pattern() = default;


		/** @{ */
		Pattern(Pattern&&) = default;
		Pattern& operator=(Pattern&&) = default;

		Pattern(const Pattern&) = delete;
		Pattern& operator=(const Pattern&) = delete;
		/** @} */


		/** @{ */
		/** @brief Rightmost trailing atom. */
		const CharAtom* endAtom() const noexcept;


		/** @brief Atoms that can 'follow' the atom given. */
		AtomList followAtoms(const CharAtom* ch) const;
		

		/** @brief Atoms that come first in any string described by the pattern. */
		AtomList rootAtoms() const;
		

		/** @brief All atoms constituting the pattern. */
		AtomList atoms() const;
		/** @} */
		
		
		/** @{ */
		const ExprNode* regex() const noexcept {
			return m_regex.get();
		}

		std::string_view name() const noexcept {
			return m_name;
		}
		/** @} */


	private:
		
		static ExprPtr parseRegex(std::string_view regex);


		mutable ExprPtr m_regex;
		std::string m_name;

	};

}

#endif