#ifndef PARSEC_CORE_INDEX_HEADER
#define PARSEC_CORE_INDEX_HEADER

#include <cstddef>
#include <vector>

namespace parsec {

	/**
	 * @brief Denotes an integer index into a contiguous memory area.
	*/
	using Index = std::ptrdiff_t;



	/**
	 * @brief List of integer indices.
	*/
	using IndexList = std::vector<Index>;



	/**
	 * @brief Integer identifier.
	*/
	using Id = Index;

}

#endif