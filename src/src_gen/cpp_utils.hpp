#ifndef PARSEC_SRC_GEN_CPP_UTILS_HEADER
#define PARSEC_SRC_GEN_CPP_UTILS_HEADER

#include <string>
#include <string_view>
#include <span>

namespace parsec::src_gen::cpp_utils {

	std::string makeEnum(std::string_view name, std::span<const std::string_view> values);

}

#endif