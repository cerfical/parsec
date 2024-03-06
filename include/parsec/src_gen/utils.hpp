#ifndef PARSEC_SRC_GEN_UTILS_ROOT_HEADER
#define PARSEC_SRC_GEN_UTILS_ROOT_HEADER

#include <string>
#include <string_view>

/**
 * @brief A set of utilities to facilitate source code generation.
*/
namespace parsec::src_gen::utils {

	/**
	 * @brief Transform a valid string identifier to its unique case-insensitive representation.
	*/
	std::string normalizeName(std::string_view name);

}

#endif