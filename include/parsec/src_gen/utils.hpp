#ifndef PARSEC_SRC_GEN_UTILS_HEADER
#define PARSEC_SRC_GEN_UTILS_HEADER

#include <string>
#include <string_view>

namespace parsec::src_gen::utils {

	/**
	 * @brief Transform a valid string identifier to its unique case-insensitive representation.
	*/
	std::string normalizeName(std::string_view name);

}

#endif