#include "elr/hash.hpp"

#include "elr/ItemSet.hpp"
#include "elr/StateItem.hpp"

#include <boost/functional/hash.hpp>

template <>
struct boost::hash<parsec::elr::StateItem> {
	std::size_t operator()(const parsec::elr::StateItem& item) const noexcept {
		return parsec::elr::hash(item);
	}
};

namespace parsec::elr {
	std::size_t hash(const StateItem& item) noexcept {
		return boost::hash_value(std::tuple(item.dfaState(), item.backLink()));
	}

	std::size_t hash(const ItemSet& items) noexcept {
		return boost::hash_value(items.m_items);
	}
}