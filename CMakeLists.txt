
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
		"include/parsec/core/IndexRange.hpp"
		"include/parsec/core/LineInfo.hpp"
		"include/parsec/core/EofError.hpp"

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
		"include/parsec/core/NonCopyable.hpp"
		"include/parsec/core/NonMovable.hpp"
		"include/parsec/core/typedefs.hpp"

	PRIVATE
		"src/core/Pattern.cpp"
		"src/core/Grammar.cpp"
	
		"src/core/TextScanner.cpp"
		"src/core/TokenKinds.cpp"
		"src/core/Token.cpp"
		"src/core/Lexer.cpp"
		"src/core/Parser.cpp"

		"src/core/CppSrcGen.cpp"
)


target_sources(parsec-lib
	PUBLIC FILE_SET HEADERS BASE_DIRS "include" FILES
		"include/parsec/pars.hpp"
		"include/parsec/pars/ParseError.hpp"
		"include/parsec/pars/TokenKinds.hpp"
		"include/parsec/pars/Token.hpp"
		"include/parsec/pars/Lexer.hpp"
		"include/parsec/pars/Parser.hpp"

		"include/parsec/pars/ast.hpp"
		"include/parsec/pars/ast/Node.hpp"
		"include/parsec/pars/ast/NodeVisitor.hpp"
		"include/parsec/pars/ast/UnaryNode.hpp"
		"include/parsec/pars/ast/BinaryNode.hpp"
		"include/parsec/pars/ast/NilNode.hpp"
		"include/parsec/pars/ast/NodeList.hpp"
		"include/parsec/pars/ast/InlinePattern.hpp"
		"include/parsec/pars/ast/NamedPattern.hpp"
		"include/parsec/pars/ast/NilRule.hpp"
		"include/parsec/pars/ast/NameRule.hpp"
		"include/parsec/pars/ast/NamedRule.hpp"
		"include/parsec/pars/ast/ConcatRule.hpp"
		"include/parsec/pars/ast/AlternRule.hpp"
		"include/parsec/pars/ast/OptionalRule.hpp"
		"include/parsec/pars/ast/PlusRule.hpp"
		"include/parsec/pars/ast/StarRule.hpp"

	PRIVATE
		"src/pars/TokenKinds.cpp"
		"src/pars/Token.cpp"
		"src/pars/Lexer.cpp"
		"src/pars/Parser.cpp"
		
		"src/pars/ast.cpp"
)


target_sources(parsec-lib
	PUBLIC FILE_SET HEADERS BASE_DIRS "include" FILES
		"include/parsec/fg.hpp"
		"include/parsec/fg/Rule.hpp"
		"include/parsec/fg/SymbolGrammar.hpp"

	PRIVATE
		"src/fg/Rule.cpp"
		"src/fg/Rule-nodes.hpp"
		"src/fg/SymbolGrammar.cpp"
)


target_sources(parsec-lib
	PUBLIC FILE_SET HEADERS BASE_DIRS "include" FILES
		"include/parsec/fsm.hpp"
		"include/parsec/fsm/Automaton.hpp"
		"include/parsec/fsm/AutomatonBuilder.hpp"
		"include/parsec/fsm/AutomatonFactory.hpp"

	PRIVATE
		"src/fsm/AutomatonBuilder.cpp"
		"src/fsm/Automaton.cpp"
		"src/fsm/AutomatonFactory.cpp"
)


target_sources(parsec-lib
	PUBLIC FILE_SET HEADERS BASE_DIRS "include" FILES
		"include/parsec/src_gen.hpp"
		"include/parsec/src_gen/ConfigStore.hpp"
		"include/parsec/src_gen/CodeGen.hpp"
		"include/parsec/src_gen/CppLexerGen.hpp"
		"include/parsec/src_gen/CppParserGen.hpp"

	PRIVATE
		"src/src_gen/cpp_utils.hpp"
		"src/src_gen/cpp_utils.cpp"
		"src/src_gen/CppLexerGen.cpp"
		"src/src_gen/CppParserGen.cpp"
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
		"include/parsec/utils/string_utils.hpp"
		"include/parsec/utils/char_utils.hpp"

	PRIVATE
		"src/utils/string_utils.cpp"
		"src/utils/char_utils.cpp"
)


target_sources(parsec-lib
	PUBLIC FILE_SET HEADERS BASE_DIRS "include" FILES
		"include/parsec/regex.hpp"
	
		"include/parsec/regex/ast.hpp"
		"include/parsec/regex/ast/NodeVisitor.hpp"
		"include/parsec/regex/ast/NodeTraverser.hpp"
		"include/parsec/regex/ast/ExprNode.hpp"
		"include/parsec/regex/ast/CharAtom.hpp"
		"include/parsec/regex/ast/NilExpr.hpp"
		"include/parsec/regex/ast/BinaryExpr.hpp"
		"include/parsec/regex/ast/AlternExpr.hpp"
		"include/parsec/regex/ast/ConcatExpr.hpp"
		"include/parsec/regex/ast/UnaryExpr.hpp"
		"include/parsec/regex/ast/PlusExpr.hpp"
		"include/parsec/regex/ast/OptionalExpr.hpp"
		"include/parsec/regex/ast/StarExpr.hpp"
	
		"include/parsec/regex/RegularExpr.hpp"
		"include/parsec/regex/ParseError.hpp"
		"include/parsec/regex/Parser.hpp"

	PRIVATE
		"src/regex/ast/NodeTraverser.cpp"
		"src/regex/ast/ExprNode.cpp"
		"src/regex/ast/CharAtom.cpp"
		"src/regex/ast/NilExpr.cpp"
		"src/regex/ast/AlternExpr.cpp"
		"src/regex/ast/ConcatExpr.cpp"
		"src/regex/ast/PlusExpr.cpp"
		"src/regex/ast/OptionalExpr.cpp"
		"src/regex/ast/StarExpr.cpp"
		
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
