#ifndef PARSEC_REGEX_PATTERN_HEADER
#define PARSEC_REGEX_PATTERN_HEADER

#include "CharAtom.hpp"

#include <string_view>
#include <vector>

namespace parsec::regex {
	
	/**
	 * @brief Wraps a regular expression to provide useful functionality.
	 */
	class Pattern {
	public:

		using AtomList = std::vector<const CharAtom*>;



		/** @brief Compile a new unnamed pattern from a string. */
		Pattern(std::string_view regex, int id = 0)
			: Pattern("", regex, id)
		{ }


		/** @brief Compile a new named pattern from a string. */
		Pattern(std::string_view name, std::string_view regex, int id = 0)
			: Pattern(name, parseRegex(regex), id)
		{ }


		Pattern(std::string_view name, ExprPtr regex, int id = 0)
			: m_name(name), m_regex(std::move(regex)), m_id(id)
		{ }

		Pattern() = default;



		/** @private */
		Pattern(Pattern&&) = default;
		
		/** @private */
		Pattern& operator=(Pattern&&) = default;



		/** @{ */
		/** @brief Find the rightmost atom, if any. */
		const CharAtom* endAtom() const noexcept;


		/** @brief Find atoms that can follow the atom given in some string generated from the pattern. */
		AtomList followersOf(const CharAtom* ch) const;


		/** @brief Find atoms that come first in some string generated from the pattern. */
		AtomList rootAtoms() const;


		/** @brief Collect all atoms from the regular expression. */
		AtomList atoms() const;
		

		/** @brief Root node of the regular expression, if any. */
		const ExprNode* regex() const noexcept {
			return m_regex.get();
		}


		/** @brief String value uniquely identifying the pattern. */
		std::string_view name() const noexcept {
			return m_name;
		}


		/** @brief Integer value uniquely identifying the pattern. */
		int id() const noexcept {
			return m_id;
		}
		/** @} */



	private:

		static ExprPtr parseRegex(std::string_view regex);
		
		Pattern(const Pattern&) = delete;
		Pattern& operator=(const Pattern&) = delete;


		std::string m_name;
		ExprPtr m_regex;
		int m_id = {};
	};

}

#endif