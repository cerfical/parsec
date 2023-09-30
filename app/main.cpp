#include <parsec/parsec.hpp>
#include <fstream>

int LogError(parsec::ErrorLogger& errors) noexcept {
	try {
		throw;	
	} catch(const parsec::ParseError& err) {
		errors.Log(err.what(), err.location());
	} catch(const std::exception& err) {
		errors.Log(err.what());
	} catch(...) {
		errors.Log("something unexpected happened");
	}
	return 1;
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
		const auto grammar = parser.Parse();
		for(const auto& tok : grammar->GetTokens()) {
			const auto regex = parsec::RegExParser().Parse(tok.GetRegex());
			const auto dfa = parsec::DfaBuilder(*regex).Build();
			std::cout << "DFA for \"" << tok.GetRegex() << "\":\n";
			dfa.Dump();
		}
	} catch(...) {
		return LogError(errors);
	}
}

/*

	token = ident '=' regex ';'
	token-list = tokens '{' token* '}'

	grammar = token-list

*/