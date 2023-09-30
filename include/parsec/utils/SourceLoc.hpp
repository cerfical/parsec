#ifndef PARSEC_UTILS_SOURCE_LOC_HEADER
#define PARSEC_UTILS_SOURCE_LOC_HEADER

#include <ostream>

namespace parsec {
	/**
	 * @brief Describes a location in the source code.
	 */
	class SourceLoc {
	public:
		/** @{ */
		/** @brief Print out a location to a @c std::ostream. */
		friend std::ostream& operator<<(std::ostream& out, const SourceLoc& loc);
		/** @} */


		/** @{ */
		/** @brief Construct an empty location. */
		SourceLoc() = default;

		/** @brief Construct a new location. */
		SourceLoc(int startCol, int colCount = 1, int line = 0, int pos = 0) noexcept
			: m_startCol(startCol), m_colCount(colCount), m_line(line), m_pos(pos)
		{ }
		/** @} */


		/** @{ */
		SourceLoc(const SourceLoc&) = default;
		SourceLoc& operator=(const SourceLoc&) = default;
		/** @} */


		/** @{ */
		/** @brief Positional index of the location. */
		int pos() const noexcept {
			return m_pos;
		}

		/** @brief Line number of the location. */
		int line() const noexcept {
			return m_line;
		}

		/** @brief Number of columns spanning the location. */
		int colCount() const noexcept {
			return m_colCount;
		}

		/** @brief Starting column of the location. */
		int startCol() const noexcept {
			return m_startCol;
		}

		/** @brief Ending column of the location. */
		int endCol() const noexcept {
			return startCol() + colCount();
		}
		/** @} */

	private:
		int m_startCol = { };
		int m_colCount = { };
		int m_line = { };
		int m_pos = { };
	};
}

#endif