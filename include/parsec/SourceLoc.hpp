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
		/** @brief Prints out a location to a @c std::ostream. */
		friend std::ostream& operator<<(std::ostream& out, const SourceLoc& loc);
		/** @} */


		/** @{ */
		/** @brief Construct a new empty location. */
		SourceLoc() = default;

		/** @brief Construct a new location. */
		SourceLoc(int startCol, int colCount, int lineNo, int linePos) noexcept
			: m_startCol(startCol)
			, m_colCount(colCount)
			, m_lineNo(lineNo)
			, m_linePos(linePos)
		{ }
		/** @} */


		/** @{ */
		SourceLoc(const SourceLoc&) = default;
		SourceLoc& operator=(const SourceLoc&) = default;
		/** @} */


		/** @{ */
		/** @brief Positional index of the location. */
		int linePos() const noexcept {
			return m_linePos;
		}


		/** @brief Line number of the location. */
		int lineNo() const noexcept {
			return m_lineNo;
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
			return startCol() + colCount() - 1;
		}


		/** @brief Absolute position of the location. */
		int pos() const noexcept {
			return linePos() + startCol();
		}


		/** @brief Checks if the location contains any useful information. */
		bool isEmpty() const noexcept {
			return m_colCount == 0;
		}
		/** @} */

		
	private:
		int m_startCol = 0;
		int m_colCount = 0;
		int m_lineNo = 0;
		int m_linePos = 0;
	};
}

#endif