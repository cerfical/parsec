# Write project information to a header file
configure_file("config.hpp.in" "config.hpp")


# Define the library from the source files
add_library(parsec-lib STATIC
	"regex/ExprNode.cpp"

	"regex/NilExpr.cpp"
	"regex/CharLiteral.cpp"

	"regex/ConcatExpr.cpp"
	"regex/AlternExpr.cpp"

	"regex/PlusExpr.cpp"
	"regex/StarExpr.cpp"
	"regex/OptionalExpr.cpp"

	"regex/regex.NodePrinter.cpp"
	"regex/regex.Parser.cpp"
	

	"fg/SyntaxNode.cpp"
	"fg/RuleRef.cpp"
	"fg/BinaryNode.cpp"
	"fg/AlternNode.cpp"
	"fg/ConcatNode.cpp"

	"fg/Grammar.cpp"
	"fg/SyntaxRule.cpp"
	"fg/TokenRule.cpp"

	"fg/Token.cpp"
	"fg/Lexer.cpp"
	"fg/fg.NodePrinter.cpp"
	"fg/fg.Parser.cpp"


	"fg/rules/RuleNode.cpp"
	"fg/rules/NilRule.cpp"

	"fg/rules/RulePos.cpp"
	"fg/rules/CharLiteral.cpp"
	"fg/rules/RuleRef.cpp"

	"fg/rules/RuleConcat.cpp"
	"fg/rules/RuleAltern.cpp"

	"fg/rules/PlusRule.cpp"
	"fg/rules/StarRule.cpp"
	"fg/rules/OptRule.cpp"

	"fg/rules/RulePrinter.cpp"


	"gen/LexerGenerator.cpp"
	"gen/CppLexerGenerator.cpp"


	"utils/TextScanner.cpp"
	"utils/SourceLoc.cpp"
	"utils/chars.cpp"


	"cli/ConsoleApp.cpp"
	"cli/OptionBuilder.cpp"
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
	
	"regex/NodePrinter.hpp"
	"regex/Parser.hpp"


	"fg/fg.hpp"
	"fg/nodes.hpp"

	"fg/NodeTraverser.hpp"
	"fg/SyntaxNode.hpp"
	"fg/BinaryNode.hpp"
	"fg/AlternNode.hpp"
	"fg/ConcatNode.hpp"

	"fg/GrammarRule.hpp"
	"fg/TokenRule.hpp"
	"fg/SyntaxRule.hpp"
	"fg/Grammar.hpp"
	
	"fg/NodePrinter.hpp"
	"fg/Token.hpp"
	"fg/Lexer.hpp"
	"fg/Parser.hpp"


	"fg/rules/rules.hpp"

	"fg/rules/RuleNode.hpp"
	"fg/rules/NilRule.hpp"
	"fg/rules/RuleTraverser.hpp"

	"fg/rules/RulePos.hpp"
	"fg/rules/CharLiteral.hpp"
	"fg/rules/RuleRef.hpp"
	
	"fg/rules/BinaryRule.hpp"
	"fg/rules/RuleConcat.hpp"
	"fg/rules/RuleAltern.hpp"
	
	"fg/rules/UnaryRule.hpp"
	"fg/rules/PlusRule.hpp"
	"fg/rules/StarRule.hpp"
	"fg/rules/OptRule.hpp"
	
	"fg/rules/RulePrinter.hpp"


	"gen/gen.hpp"
	"gen/LexerGenerator.hpp"
	"gen/CppLexerGenerator.hpp"


	"utils/utils.hpp"
	"utils/chars.hpp"

	"utils/TextScanner.hpp"
	"utils/SourceLoc.hpp"
	"utils/ParseError.hpp"


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
