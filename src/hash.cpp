#include "hash.hpp"

#include "core/Symbol.hpp"

#include "elr/StateItem.hpp"
#include "elr/ItemSet.hpp"

#include <boost/functional/hash.hpp>

template <>
struct boost::hash<parsec::elr::StateItem> {
	std::size_t operator()(const parsec::elr::StateItem& item) const noexcept {
		return parsec::hash(item);
	}
};

namespace parsec {
	std::size_t hash(const class Symbol& symbol) noexcept {
		return std::hash<std::string>()(symbol.value());
	}



	std::size_t hash(const elr::StateItem& item) noexcept {
		return boost::hash_value(std::tuple(item.dfaState(), item.backLink()));
	}

	std::size_t hash(const elr::ItemSet& items) noexcept {
		return boost::hash_value(items.m_items);
	}
}