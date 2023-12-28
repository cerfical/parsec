#ifndef PARSEC_CORE_LINE_INFO_HEADER
#define PARSEC_CORE_LINE_INFO_HEADER

#include <gsl/util>

namespace parsec {

	/**
	 * @brief Information on one line of multiline text.
	*/
	struct LineInfo {

		LineInfo() = default;

		LineInfo(gsl::index no, gsl::index pos) noexcept
			: no(no), pos(pos)
		{ }



		/**
		 * @brief Ordinal line number.
		*/
		gsl::index no = {};



		/**
		 * @brief Absolute line position.
		*/
		gsl::index pos = {};

	};

}

#endif