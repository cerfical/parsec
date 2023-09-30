#include "Utils.hpp"

#include <boost/algorithm/string/trim.hpp>
#include <iomanip>

namespace {
	using namespace parsec;

	std::string BuildFooterTail(const SourceLocation& loc, std::ptrdiff_t off) {
		// construct a mark to visually highlight the location
		const std::string mark(loc.GetColumnCount(), loc.GetColumnCount() != 1 ? '~' : '^');

		// construct a string of spaces to move the mark to the proper place
		const std::string spaces(loc.GetColumnNo() + off, ' ');
		return spaces + mark;
	}
}

namespace parsec {
	std::string CharUtils::EscapeChar(char ch) {
		if(IsPrint(ch)) {
			switch(ch) {
				case '\\': return "\\\\";
				case '\'': return "\\\'";
				default: {
					return std::string(1, ch);
				}
			}
		} else {
			switch(ch) {
				case '\0': return "\\\0";
				case '\a': return "\\\a";
				case '\b': return "\\\b";
				case '\f': return "\\\f";
				case '\n': return "\\\n";
				case '\r': return "\\\r";
				case '\t': return "\\\t";
				case '\v': return "\\\v";
				default: {
					return (std::ostringstream()
						<< "\\x" << std::setw(2) << std::setfill('0') << std::hex << static_cast<int>(ch)
					).str();
				}
			}
		}
	}


	std::string ErrorLogger::ReadLine(const SourceLocation& loc) {
		if(loc.IsEmpty()) {
			return "";
		}

		// save the current input position and then update the input pointer
		const auto inputPos = input->tellg();
		input->seekg(loc.GetStartPos());
		
		// read the line of text represented by the location
		std::string line;
		std::getline(*input, line);
		input->seekg(inputPos);

		return line;
	}

	void ErrorLogger::Log(std::string_view msg, const SourceLocation& loc) noexcept {
		// retrieve the location text if any
		const auto line = boost::algorithm::trim_right_copy(ReadLine(loc));
		const auto footerHead = boost::algorithm::trim_left_copy(line);

		// simple lambda to print out the location information if available
		const auto PrintLocation = [&, this]() {
			*out << filename << ':';
			if(!loc.IsEmpty()) {
				*out << loc.GetLineNo() + 1 << ':' << loc.GetColumnNo() + 1 << ':';
			}
		};

		// simple lambda to print an error message
		const auto PrintError = [&, this]() {
			*out << " error: " << msg;
			if(!footerHead.empty()) {
				*out << ':';
			}
			*out << '\n';
		};

		// simple lambda to print an optional message footer
		const auto PrintFooter = [&, this]() {
			if(!footerHead.empty()) {
				const auto footerTail = BuildFooterTail(loc, footerHead.size() - line.size());
				*out << "    " << footerHead << '\n';
				*out << "    " << footerTail << '\n';
			}
		};

		PrintLocation();
		PrintError();
		PrintFooter();
	}
}