#include "hash.hpp"

#include "core/Symbol.hpp"

namespace parsec {
	std::size_t hash(const class Symbol& symbol) noexcept {
		return std::hash<std::string>()(symbol.value());
	}
}