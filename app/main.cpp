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
	
}
