#include <parsec/parsec.hpp>
#include <fstream>

int Error(parsec::ErrorLogger &errors) noexcept {
	try {
		throw;	
	} catch(const parsec::ParseError& err) {
		errors.Log(err.what(), err.location());
		return 1;
	} catch(const std::exception& err) {
		errors.Log(err.what());
		return 1;
	} catch(...) {
		errors.Log("something VERY unexpected happened");
		return 1;
	}
}

int main(int argc, gsl::czstring argv[]) {
	if(const auto program = argv[0]; argc != 2) {
		std::cerr << "Usage: " << program << " <filename>" << '\n';
		return 1;
	}

	const auto filename = argv[1];
	std::ifstream fin(filename, std::ios_base::binary);

	parsec::ErrorLogger errors(fin, filename);
	if(!fin.is_open()) {
		errors.Log("file not found");
		return 1;
	}

	parsec::BnfParser parser(fin);
	try {
		auto grammar = parser.Parse();
		for(const auto& tok : grammar->GetTokens()) {
			std::cout << "token: "<< tok.GetName() << " = " << tok.GetRegex() << std::endl;
		}
	} catch(...) {
		return Error(errors);
	}
}

/*

	token = ident '=' regex ';'
	token-list = tokens '{' token* '}'

	grammar = token-list

*/