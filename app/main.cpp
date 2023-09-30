#include <parsec/parsec.hpp>
#include <iostream>
#include <sstream>

int main() {
	std::istringstream input(R"(
	tokens {
		left-paren = '(';
		right-paren = ')';
		ident = '<letter>*';

		'ab(0|1)'
	})");

	parsec::Lexer lexer(input);
	while(true) {
		const auto tok = lexer.Lex();
		if(tok.IsEof()) {
			break;
		}
		std::cout << tok.GetText() << std::endl;
	}
}

/*

PrintRegex("a|b(");

void PrintRegex(std::string_view regex) {
	parsec::RegExParser parser;
	try {
		const auto rgx = parser.Parse(regex);
		rgx->Print();
	} catch(const parsec::ParseError &err) {
		std::cout << "error: " << err.what() << ":\n";
		if(const auto& loc = err.location(); !loc.IsEmpty()) {
			std::cout << "    " << regex << '\n';
			std::cout << "    " << std::string(loc.GetStartPos(), ' ')
				<< ((loc.GetSize() != 1) ? 
					std::string(loc.GetSize(), '~')
					 : std::string(1, '^'))
				<< '\n';
		}
	}
}

*/