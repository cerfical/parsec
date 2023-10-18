# Write project information to a header file
configure_file("config.hpp.in" "config.hpp")


# Define the library from the source files
add_library(parsec-lib STATIC
	"regex/ExprNode.cpp"

	"regex/NilExpr.cpp"
	"regex/CharLiteral.cpp"

	"regex/BinaryExpr.cpp"
	"regex/ConcatExpr.cpp"
	"regex/AlternExpr.cpp"

	"regex/UnaryExpr.cpp"
	"regex/PlusExpr.cpp"
	"regex/StarExpr.cpp"
	"regex/OptionalExpr.cpp"

	"regex/Printer.cpp"
	"regex/Parser.cpp"
	

	"fg/LangGrammar.cpp"
	
	"fg/SyntaxRule.cpp"
	"fg/TokenRule.cpp"
	"fg/Token.cpp"
	"fg/Lexer.cpp"
	"fg/Parser.cpp"

	"gen/LexerGenerator.cpp"
	"gen/CppLexerGenerator.cpp"

	"utils/TextScanner.cpp"
	"utils/SourceLoc.cpp"
	"utils/chars.cpp"

	"cli/ConsoleApp.cpp"
	"cli/ConsoleApp.OptionBuilder.cpp"
)


# List header files for the library
set(_HEADERS
	"parsec.hpp"
		
	"regex/regex.hpp"
	"regex/nodes.hpp"

	"regex/ExprNode.hpp"
	"regex/NodeTraverser.hpp"
	
	"regex/NilExpr.hpp"
	"regex/CharLiteral.hpp"
	
	"regex/BinaryExpr.hpp"
	"regex/ConcatExpr.hpp"
	"regex/AlternExpr.hpp"
	
	"regex/UnaryExpr.hpp"
	"regex/PlusExpr.hpp"
	"regex/StarExpr.hpp"
	"regex/OptionalExpr.hpp"
	
	"regex/Printer.hpp"
	"regex/Parser.hpp"

	"fg/fg.hpp"
	"fg/LangGrammar.hpp"
	"fg/GrammarRule.hpp"
	"fg/GrammarNode.hpp"
	"fg/SyntaxRule.hpp"
	"fg/TokenRule.hpp"
	"fg/Token.hpp"
	"fg/Lexer.hpp"
	"fg/Parser.hpp"
	"fg/nodes.hpp"

	"gen/gen.hpp"
	"gen/LexerGenerator.hpp"
	"gen/CppLexerGenerator.hpp"

	"utils/utils.hpp"
	"utils/TextScanner.hpp"
	"utils/SourceLoc.hpp"
	"utils/ParseError.hpp"
	"utils/chars.hpp"

	"cli/cli.hpp"
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
	PUBLIC Boost::program_options
	PRIVATE Boost::boost
)

set_target_properties(parsec-lib PROPERTIES OUTPUT_NAME "parsec")
