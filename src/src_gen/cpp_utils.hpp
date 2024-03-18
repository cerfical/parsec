#ifndef PARSEC_SRC_GEN_CPP_UTILS_HEADER
#define PARSEC_SRC_GEN_CPP_UTILS_HEADER

#include "fg/Symbol.hpp"

#include <string>
#include <span>

namespace parsec::src_gen::cpp_utils {

	std::string makeEnum(const fg::Symbol& name, std::span<const fg::Symbol> values);

}

#endif