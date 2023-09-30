#include "PrintUtils.hpp"

#include <boost/algorithm/string.hpp>
#include <iostream>

void PrintParseError(const parsec::ParseError& err, std::istream& input, gsl::czstring filename) noexcept {
	// save the current input position and then update the input pointer
	const auto inputPos = input.tellg();
	const auto& loc = err.location();
	input.seekg(loc.GetStartPos());

	// read the line of text represented by the location
	std::string line;
	std::getline(input, line);

	// strip any leading spaces from the line
	const auto trimmedLine = boost::algorithm::trim_left_copy(line);

	// construct a string of spaces to move the mark to the proper place in the printed output
	const auto spacesRemoved = line.size() -  trimmedLine.size();
	const std::string spaces(loc.GetColumnNo() - spacesRemoved, ' ');

	// construct a mark to underline the location in the line
	const std::string mark(loc.GetColumnCount(), loc.GetColumnCount() != 1 ? '~' : '^');

	PrintError(filename, loc.GetLineNo(), ':', loc.GetColumnNo(), ": error: ", err.what(), ':');
	PrintError("    ", trimmedLine);
	PrintError("    ", spaces, mark);
	
	// move the input pointer to its original position
	input.seekg(inputPos);
}