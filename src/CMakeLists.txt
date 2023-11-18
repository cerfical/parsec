# Write project information to a header file
configure_file("config.hpp.in" "config.hpp")


# Define the library from the source files
add_library(parsec-lib STATIC
	"Pattern.cpp"
	"Grammar.cpp"
	
	"TextScanner.cpp"
	"SourceLoc.cpp"

	"TokenKinds.cpp"
	"Token.cpp"
	"Lexer.cpp"

	"Parser.cpp"
	"CppSrcGen.cpp"


	"regex/ExprNode.cpp"
	"regex/ExprTraverser.cpp"
	"regex/ExprPrinter.cpp"

	"regex/CharAtom.cpp"
	"regex/NilExpr.cpp"

	"regex/AlternExpr.cpp"
	"regex/ConcatExpr.cpp"

	"regex/PlusExpr.cpp"
	"regex/OptionalExpr.cpp"
	"regex/StarExpr.cpp"

	"regex/Parser.cpp"

	
	"lr/StateGen.cpp"
	"dfa/StateGen.cpp"

	"utils/Strings.cpp"
	"utils/Chars.cpp"

	"cli/ConsoleApp.cpp"
	"cli/OptionBuilder.cpp"
)


# List header files for the library
set(_HEADERS
	"parsec.hpp"


	"Rule.hpp"
	"Symbol.hpp"
	"Pattern.hpp"
	"CharAtom.hpp"
	"Grammar.hpp"

	"TextScanner.hpp"
	"SourceLoc.hpp"
	"Error.hpp"

	"TokenKinds.hpp"
	"Token.hpp"
	"Lexer.hpp"
	
	"Parser.hpp"
	"CppSrcGen.hpp"


	"regex/nodes.hpp"
	
	"regex/ExprNode.hpp"
	"regex/NodeVisitor.hpp"
	"regex/ExprTraverser.hpp"
	"regex/ExprPrinter.hpp"

	"regex/CharAtom.hpp"
	"regex/NilExpr.hpp"

	"regex/BinaryExpr.hpp"
	"regex/AlternExpr.hpp"
	"regex/ConcatExpr.hpp"

	"regex/UnaryExpr.hpp"
	"regex/PlusExpr.hpp"
	"regex/OptionalExpr.hpp"
	"regex/StarExpr.hpp"

	"regex/Parser.hpp"


	"lr/StateGoto.hpp"
	"lr/StateShift.hpp"
	"lr/StateReduce.hpp"
	"lr/State.hpp"
	"lr/StateGen.hpp"
	
	"dfa/Transition.hpp"
	"dfa/State.hpp"
	"dfa/StateGen.hpp"

	"utils/Strings.hpp"
	"utils/Chars.hpp"

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
