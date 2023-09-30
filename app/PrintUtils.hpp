#ifndef PARSEC_PRINT_UTILS_HEADER
#define PARSEC_PRINT_UTILS_HEADER

#include <parsec/parsec.hpp>
#include <gsl/gsl>

#include <istream>
#include <sstream>

/** @brief Print out a ParseError to the @c std::cerr, including its description and some contextual information. */
void PrintParseError(const parsec::ParseError& err, std::istream& input, gsl::czstring filename) noexcept;

/** @brief Combine a list of values into a single error message and print it to the @c std::cerr. */
template <typename... Args>
void PrintError(Args&&... args) noexcept {
	std::cerr << ((std::ostringstream() << ... << std::forward<Args>(args)) << '\n').str();
}

#endif