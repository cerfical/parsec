#ifndef PARSEC_CORE_SOURCE_LOC_HEADER
#define PARSEC_CORE_SOURCE_LOC_HEADER

#include "IndexRange.hpp"
#include "LineInfo.hpp"

namespace parsec {

	/**
	 * @brief One-line location in source text.
	*/
	struct SourceLoc {
		
		SourceLoc() = default;

		SourceLoc(const LineInfo& line, const IndexRange& cols) noexcept
			: line(line), cols(cols)
		{ }



		/**
		 * @brief Text line of interest.
		*/
		LineInfo line;
		


		/**
		 * @brief Contiguous range of characters within a line.
		*/
		IndexRange cols;

	};

}

#endif