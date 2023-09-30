#include <parsec/parsec.hpp>
#include <iostream>

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

int main() {
	PrintRegex("a|b(");
}

/*

	tokens {
		string-literal = '\'<letter>*\'';
		left-paren = '(';
		right-paren = ')';
		ident = '<letter>*';

		'ab(0|1)'
	}

	syntax {
		func-def = ident ident '(' ')';
		var-def = ident ident;
	}

*/