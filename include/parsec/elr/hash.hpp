#ifndef PARSEC_ELR_HASH_HEADER
#define PARSEC_ELR_HASH_HEADER

#include <cstddef>

namespace parsec::elr {

	std::size_t hash(const class StateItem& item) noexcept;
	
	std::size_t hash(const class ItemSet& items) noexcept;

}

#endif