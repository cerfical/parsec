# Write project information to a header file
configure_file("../include/parsec/Config.hpp.in" "Config.hpp")


# Define the library from the source files
add_library(parsec-lib STATIC
	"Utils.cpp"
	"Grammar.cpp"

	"LexGen.cpp"
	"CppLexGen.cpp"

	"RegExNodes.cpp"
	"RegExPrinter.cpp"
	"RegExParser.cpp"

	"BnfToken.cpp"
	"BnfLexer.cpp"
	"BnfParser.cpp"
)


# List header files for the library
set(_HEADERS
	"parsec.hpp"
		
	"Utils.hpp"
	"Grammar.hpp"
		
	"LexGen.hpp"
	"CppLexGen.hpp"
		
	"RegExNodes.hpp"
	"RegExPrinter.hpp"
	"RegExParser.hpp"
		
	"BnfToken.hpp"
	"BnfLexer.hpp"
	"BnfParser.hpp"
)
list(TRANSFORM _HEADERS PREPEND "../include/parsec/")

target_sources(parsec-lib
	PUBLIC FILE_SET HEADERS
	FILES ${_HEADERS}
	BASE_DIRS "../include"
)


# Specify build and usage requirements
target_include_directories(parsec-lib
	PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>"
	PRIVATE "../include/parsec"
)

target_compile_features(parsec-lib PUBLIC cxx_std_20)
target_link_libraries(parsec-lib
	PUBLIC Microsoft.GSL::GSL
	PRIVATE Boost::boost
)

set_target_properties(parsec-lib PROPERTIES OUTPUT_NAME "parsec")
