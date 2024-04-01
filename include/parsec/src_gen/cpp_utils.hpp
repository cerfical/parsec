#ifndef PARSEC_SRC_GEN_CPP_UTILS_HEADER
#define PARSEC_SRC_GEN_CPP_UTILS_HEADER

#include "../fg/Symbol.hpp"

#include <string>
#include <span>

/**
 * @brief Auxiliary functions to facilitate C++ source generation.
*/
namespace parsec::src_gen::cpp_utils {

	/**
	 * @brief Make up the source code for a C++ enumeration.
	 * @param enumName Name of the enumeration.
	 * @param valueNames List of names for the enumeration values.
	*/
	std::string makeEnum(const fg::Symbol& enumName, std::span<const fg::Symbol> valueNames);

}

#endif