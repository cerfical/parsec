#ifndef PARSEC_SOURCE_LOC_HEADER
#define PARSEC_SOURCE_LOC_HEADER

#include <ostream>

namespace parsec {
	/**
	 * @brief Describes a location in the source code.
	 */
	class SourceLoc {
	public:
		/** @{ */
		friend std::ostream& operator<<(std::ostream& out, const SourceLoc& loc);
		/** @} */


		/** @{ */
		SourceLoc() = default;

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
		int linePos() const noexcept {
			return m_linePos;
		}

		int lineNo() const noexcept {
			return m_lineNo;
		}
		/** @} */


		/** @{ */
		int colCount() const noexcept {
			return m_colCount;
		}

		int startCol() const noexcept {
			return m_startCol;
		}

		int endCol() const noexcept {
			return startCol() + colCount();
		}
		/** @} */


		/** @{ */
		bool isEmpty() const noexcept {
			return m_colCount == 0;
		}
		
		int pos() const noexcept {
			return linePos() + startCol();
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