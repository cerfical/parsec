#include "PrintUtils.hpp"

#include <parsec/parsec.hpp>
#include <fstream>

/** @brief Parse the command line arguments for the name of the file to be processed. */
gsl::czstring ParseCmdArgs(int argc, gsl::czstring* argv) noexcept {
	if(argc != 2) {
		PrintError("Usage: ", argv[0], " <filename>");
		std::exit(1);
	}
	return argv[1];
}

/** @brief Parse the file with the given name. */
void ParseFile(gsl::czstring filename) {
	std::ifstream fin(filename, std::ios_base::binary);
	if(!fin.is_open()) {
		PrintError("error: file not found: ", '\"', filename, '\"');
		std::exit(1);
	}
	
	parsec::BnfParser parser(fin);
	try {
		parser.Parse();
	} catch(const parsec::ParseError& err) {
		PrintParseError(err, fin, filename);
		std::exit(1);
	}
}

int main(int argc, gsl::czstring* argv) {
	try {
		const auto filename = ParseCmdArgs(argc, argv);
		ParseFile(filename);
	} catch(const std::exception& err) {
		PrintError("error: ", err.what());
	} catch(...) {
		PrintError("unknown error has occurred");
	}
}

/*

	token-def = ident '=' regex ';'
	token-list = tokens '{' token-def* '}'

	grammar-def = token-list

*/