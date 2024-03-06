#include "src_gen/utils.hpp"
#include "utils/string_utils.hpp"

namespace parsec::src_gen::utils {
	std::string normalizeName(std::string_view name) {
		return string_utils::toPascalCase(name);
	}
}