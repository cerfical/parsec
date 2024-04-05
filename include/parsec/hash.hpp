#ifndef PARSEC_HASH_HEADER
#define PARSEC_HASH_HEADER

#include <cstddef>

namespace parsec {

	class Symbol;

	namespace elr {
		class StateItem;
		class ItemSet;
	}


	/**
	 * @name Finding hash values for objects
	 * @{
	*/
	std::size_t hash(const Symbol& symbol) noexcept;

	std::size_t hash(const elr::StateItem& item) noexcept;

	std::size_t hash(const elr::ItemSet& items) noexcept;
	/** @} */

}

#endif