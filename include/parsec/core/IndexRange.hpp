#ifndef PARSEC_CORE_INDEX_RANGE_HEADER
#define PARSEC_CORE_INDEX_RANGE_HEADER

#include <gsl/util>

namespace parsec {

	/**
	 * @brief Specifies a range of elements referenced by a range of indices.
	*/
	struct IndexRange {

		IndexRange() = default;

		IndexRange(gsl::index start, gsl::index end) noexcept
			: start(start), end(end)
		{ }



		/**
		 * @brief Create a range containing one element.
		*/
		IndexRange(gsl::index start) noexcept
			: IndexRange(start, start + 1)
		{ }



		/**
		 * @brief Number of elements in the range.
		*/
		std::size_t size() const noexcept {
			return end - start;
		}



		/**
		 * @brief Lower bound of the range.
		*/
		gsl::index start = {};



		/**
		 * @brief Upper bound of the range.
		*/
		gsl::index end = {};

	};

}

#endif