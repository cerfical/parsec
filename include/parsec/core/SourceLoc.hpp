#ifndef PARSEC_CORE_SOURCE_LOC_HEADER
#define PARSEC_CORE_SOURCE_LOC_HEADER

namespace parsec {

	/**
	 * @brief Describes a one-line location in text.
	*/
	class SourceLoc {
	public:

		explicit SourceLoc(int startCol = 0)
			: SourceLoc(startCol, 0, 0) {}

		SourceLoc(int startCol, int lineNo, int linePos)
			: SourceLoc(startCol, 1, lineNo, linePos) {}
		
		SourceLoc(int startCol, int colCount, int lineNo, int linePos)
			: m_startCol(startCol), m_colCount(colCount), m_lineNo(lineNo), m_linePos(linePos) {}


		int linePos() const {
			return m_linePos;
		}

		int lineNo() const {
			return m_lineNo;
		}

		int colCount() const {
			return m_colCount;
		}

		int startCol() const {
			return m_startCol;
		}


		int endCol() const {
			return startCol() + colCount();
		}

		int pos() const {
			return linePos() + startCol();
		}


	private:
		int m_startCol = {};
		int m_colCount = {};
		int m_lineNo = {};
		int m_linePos = {};
	};

}

#endif