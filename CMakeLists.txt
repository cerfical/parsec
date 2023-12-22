
cmake_minimum_required(VERSION 3.23)



# Basic project information

project(parsec LANGUAGES CXX DESCRIPTION "Simple parser generator" VERSION 1.0.0)
include(CTest)

option(BUILD_APP "Build the command line tool" ON)
option(BUILD_DOC "Generate documentation from the source tree using Doxygen" OFF)
option(BUILD_EXAMPLES "Build the examples" OFF)
option(BUILD_SHARED_LIBS "Build shared libraries instead of static ones" OFF)

find_package(Microsoft.GSL REQUIRED)
find_package(Boost REQUIRED)


configure_file("src/config.hpp.in" "include/parsec/config.hpp")



# Setup the main project library

add_library(parsec-lib)

set_target_properties(parsec-lib PROPERTIES OUTPUT_NAME parsec)
target_compile_features(parsec-lib PUBLIC cxx_std_20)


target_include_directories(parsec-lib
	PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>"
	PRIVATE "include/parsec"
)

target_link_libraries(parsec-lib
	PUBLIC Microsoft.GSL::GSL
	PRIVATE Boost::boost
)



# List all source and header files for the library

target_sources(parsec-lib
	PUBLIC FILE_SET HEADERS BASE_DIRS "include" FILES
		"include/parsec/parsec.hpp"
		"include/parsec/deps.hpp"
)


target_sources(parsec-lib
	PUBLIC FILE_SET HEADERS BASE_DIRS "include" FILES
		"include/parsec/core/Error.hpp"

		"include/parsec/core/Rule.hpp"
		"include/parsec/core/Pattern.hpp"
		"include/parsec/core/Symbol.hpp"
		"include/parsec/core/CharAtom.hpp"
		"include/parsec/core/Grammar.hpp"
	
		"include/parsec/core/TextScanner.hpp"
		"include/parsec/core/SourceLoc.hpp"
		"include/parsec/core/ParseError.hpp"
		"include/parsec/core/TokenKinds.hpp"
		"include/parsec/core/TokenBase.hpp"
		"include/parsec/core/Token.hpp"
		"include/parsec/core/LexerBase.hpp"
		"include/parsec/core/Lexer.hpp"
	
		"include/parsec/core/Parser.hpp"
		"include/parsec/core/CppSrcGen.hpp"
	
	PRIVATE
		"src/core/Pattern.cpp"
		"src/core/Grammar.cpp"
	
		"src/core/TextScanner.cpp"
		"src/core/SourceLoc.cpp"
		"src/core/TokenKinds.cpp"
		"src/core/Token.cpp"
		"src/core/Lexer.cpp"
	
		"src/core/Parser.cpp"
		"src/core/CppSrcGen.cpp"
)


target_sources(parsec-lib
	PUBLIC FILE_SET HEADERS BASE_DIRS "include" FILES
		"include/parsec/lr/Transition.hpp"
		"include/parsec/lr/StateReduce.hpp"
		"include/parsec/lr/StateGen.hpp"
		"include/parsec/lr/State.hpp"
	
	PRIVATE
		"src/lr/StateGen.cpp"
)


target_sources(parsec-lib
	PUBLIC FILE_SET HEADERS BASE_DIRS "include" FILES
		"include/parsec/dfa/Transition.hpp"
		"include/parsec/dfa/StateGen.hpp"
		"include/parsec/dfa/State.hpp"

	PRIVATE
		"src/dfa/StateGen.cpp"
)


target_sources(parsec-lib
	PUBLIC FILE_SET HEADERS BASE_DIRS "include" FILES
		"include/parsec/bnf.hpp"

		"include/parsec/bnf/GrammarSymbol.hpp"
		"include/parsec/bnf/GrammarRule.hpp"
		"include/parsec/bnf/Grammar.hpp"
		"include/parsec/bnf/RegularPattern.hpp"
		"include/parsec/bnf/RegularGrammar.hpp"

	PRIVATE
		"src/bnf/GrammarRule.cpp"
		"src/bnf/Grammar.cpp"
		"src/bnf/RegularPattern.cpp"
)


target_sources(parsec-lib
	PUBLIC FILE_SET HEADERS BASE_DIRS "include" FILES
		"include/parsec/utils/Strings.hpp"
		"include/parsec/utils/Chars.hpp"

	PRIVATE
		"src/utils/Strings.cpp"
		"src/utils/Chars.cpp"
)


target_sources(parsec-lib
	PUBLIC FILE_SET HEADERS BASE_DIRS "include" FILES
		"include/parsec/regex.hpp"
		"include/parsec/regex/nodes.hpp"
	
		"include/parsec/regex/nodes/NodeVisitor.hpp"
		"include/parsec/regex/nodes/ExprNode.hpp"
		"include/parsec/regex/nodes/CharAtom.hpp"
		"include/parsec/regex/nodes/NilExpr.hpp"
		"include/parsec/regex/nodes/BinaryExpr.hpp"
		"include/parsec/regex/nodes/AlternExpr.hpp"
		"include/parsec/regex/nodes/ConcatExpr.hpp"
		"include/parsec/regex/nodes/UnaryExpr.hpp"
		"include/parsec/regex/nodes/PlusExpr.hpp"
		"include/parsec/regex/nodes/OptionalExpr.hpp"
		"include/parsec/regex/nodes/StarExpr.hpp"

		"include/parsec/regex/ExprTraverser.hpp"
		"include/parsec/regex/RegularExpr.hpp"
		"include/parsec/regex/Parser.hpp"
	
	PRIVATE
		"src/regex/nodes/ExprNode.cpp"
		"src/regex/nodes/CharAtom.cpp"
		"src/regex/nodes/NilExpr.cpp"
		"src/regex/nodes/AlternExpr.cpp"
		"src/regex/nodes/ConcatExpr.cpp"
		"src/regex/nodes/PlusExpr.cpp"
		"src/regex/nodes/OptionalExpr.cpp"
		"src/regex/nodes/StarExpr.cpp"

		"src/regex/ExprTraverser.cpp"
		"src/regex/RegularExpr.cpp"
		"src/regex/Parser.cpp"
)



# Optional components

if(BUILD_APP)
	add_subdirectory(app)
endif()

if(BUILD_TESTING)
	add_subdirectory(tests)
endif()

if(BUILD_DOC)
	add_subdirectory(docs)
endif()

if(BUILD_EXAMPLES)
	add_subdirectory(examples/calc)
endif()
