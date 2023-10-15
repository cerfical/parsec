# Write project information to a header file
configure_file("../include/parsec/config.hpp.in" "config.hpp")


# Define the library from the source files
add_library(parsec-lib STATIC
	"regex/nodes.cpp"
	"regex/ExprNode.cpp"
	"regex/Printer.cpp"
	"regex/Parser.cpp"
	"regex/RegEx.cpp"

	"fg/LangGrammar.cpp"
	"fg/Token.cpp"
	"fg/Lexer.cpp"
	"fg/Parser.cpp"

	"gen/LexerGenerator.cpp"
	"gen/CppLexerGenerator.cpp"

	"utils/TextScanner.cpp"
	"utils/SourceLoc.cpp"
	"utils/chars.cpp"

	"cli/ConsoleApp.cpp"
	"cli/OptionStore.cpp"
	"cli/OptionBuilder.cpp"
)


# List header files for the library
set(_HEADERS
	"parsec.hpp"
		
	"regex.hpp"
	"regex/Traverser.hpp"
	"regex/ExprNode.hpp"
	"regex/Printer.hpp"
	"regex/Parser.hpp"
	"regex/RegEx.hpp"
	"regex/nodes.hpp"

	"fg.hpp"
	"fg/LangGrammar.hpp"
	"fg/Token.hpp"
	"fg/Lexer.hpp"
	"fg/Parser.hpp"

	"gen.hpp"
	"gen/LexerGenerator.hpp"
	"gen/CppLexerGenerator.hpp"

	"utils.hpp"
	"utils/TextScanner.hpp"
	"utils/SourceLoc.hpp"
	"utils/ParseError.hpp"
	"utils/chars.hpp"

	"cli.hpp"
	"cli/OptionStore.hpp"
	"cli/OptionBuilder.hpp"
	"cli/ConsoleApp.hpp"
)
list(TRANSFORM _HEADERS PREPEND "../include/parsec/")

target_sources(parsec-lib
	PUBLIC FILE_SET HEADERS
	FILES ${_HEADERS}
	BASE_DIRS "../include/"
)


# Specify build and usage requirements
target_include_directories(parsec-lib
	PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>"
	PRIVATE "../include/parsec/"
)

target_compile_features(parsec-lib PUBLIC cxx_std_20)
target_link_libraries(parsec-lib
	PUBLIC Microsoft.GSL::GSL
	PRIVATE Boost::boost
)

set_target_properties(parsec-lib PROPERTIES OUTPUT_NAME "parsec")
