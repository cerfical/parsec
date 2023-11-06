# Write project information to a header file
configure_file("config.hpp.in" "config.hpp")


# Define the library from the source files
add_library(parsec-lib STATIC
	"regex/Parser.cpp"

	
	"fg/Grammar.cpp"

	"fg/Token.cpp"
	"fg/TokenKinds.cpp"
	"fg/Lexer.cpp"
	
	"fg/Parser.cpp"

	"fg/Rule.cpp"
	"fg/RulePrinter.cpp"

	"fg/Atom.cpp"
	"fg/AtomVisitor.cpp"
	"fg/NilRule.cpp"

	"fg/RuleConcat.cpp"
	"fg/RuleAltern.cpp"

	"fg/PlusRule.cpp"
	"fg/StarRule.cpp"
	"fg/OptionalRule.cpp"


	"lr/StateGen.cpp"


	"dfa/StateGen.cpp"


	"utils/TextScanner.cpp"
	"utils/SourceLoc.cpp"
	"utils/chars.cpp"


	"cli/ConsoleApp.cpp"
	"cli/OptionBuilder.cpp"


	"gen/CppSrcGen.cpp"
)


# List header files for the library
set(_HEADERS
	"parsec.hpp"


	"regex/regex.hpp"
	"regex/ParseOptions.hpp"
	"regex/Parser.hpp"


	"fg/fg.hpp"
	"fg/rules.hpp"
	"fg/Symbol.hpp"
	"fg/SymbolTypes.hpp"
	"fg/Grammar.hpp"
	
	"fg/Token.hpp"
	"fg/TokenKinds.hpp"
	"fg/Lexer.hpp"
	"fg/Parser.hpp"

	"fg/Rule.hpp"
	"fg/RuleTraverser.hpp"
	"fg/RulePrinter.hpp"

	"fg/Atom.hpp"
	"fg/AtomVisitor.hpp"
	"fg/NilRule.hpp"
	
	"fg/BinaryRule.hpp"
	"fg/RuleConcat.hpp"
	"fg/RuleAltern.hpp"
	
	"fg/UnaryRule.hpp"
	"fg/PlusRule.hpp"
	"fg/StarRule.hpp"
	"fg/OptionalRule.hpp"
	

	"lr/lr.hpp"

	"lr/ShiftAction.hpp"
	"lr/ReduceAction.hpp"
	"lr/State.hpp"
	"lr/StateGen.hpp"

	
	"dfa/dfa.hpp"

	"dfa/Match.hpp"
	"dfa/Transition.hpp"
	"dfa/State.hpp"
	"dfa/StateGen.hpp"


	"utils/utils.hpp"
	"utils/chars.hpp"

	"utils/TextScanner.hpp"
	"utils/SourceLoc.hpp"
	"utils/Error.hpp"


	"cli/cli.hpp"
	"cli/ConsoleApp.hpp"


	"gen/gen.hpp"
	"gen/CppSrcGen.hpp"
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
