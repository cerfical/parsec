
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
		"include/parsec/stringify.hpp"
		"include/parsec/print.hpp"

	PRIVATE
		"src/stringify.cpp"
		"src/print.cpp"
)


target_sources(parsec-lib
	PUBLIC FILE_SET HEADERS BASE_DIRS "include" FILES
		"include/parsec/core/Error.hpp"
		"include/parsec/core/IndexRange.hpp"
		"include/parsec/core/LineInfo.hpp"

		"include/parsec/core/TextScanner.hpp"
		"include/parsec/core/SourceLoc.hpp"
		"include/parsec/core/ParseError.hpp"
		"include/parsec/core/TokenBase.hpp"
		"include/parsec/core/LexerBase.hpp"
		"include/parsec/core/Compiler.hpp"
	
		"include/parsec/core/NonCopyable.hpp"
		"include/parsec/core/NonMovable.hpp"

	PRIVATE
		"src/core/TextScanner.cpp"
		"src/core/Compiler.cpp"
)


target_sources(parsec-lib
	PUBLIC FILE_SET HEADERS BASE_DIRS "include" FILES
		"include/parsec/errors/Error.hpp"
		"include/parsec/errors/ErrorCodes.hpp"
		"include/parsec/errors/ParseError.hpp"
		"include/parsec/errors/UnexpectedEofError.hpp"
		"include/parsec/errors/InvalidCharError.hpp"
		"include/parsec/errors/UnexpectedTokenError.hpp"
		"include/parsec/errors/TokenMismatchError.hpp"
		"include/parsec/errors.hpp"

	PRIVATE
		"src/errors.cpp"
)


target_sources(parsec-lib
	PUBLIC FILE_SET HEADERS BASE_DIRS "include" FILES
		"include/parsec/pars.hpp"
		"include/parsec/pars/TokenKinds.hpp"
		"include/parsec/pars/Token.hpp"
		"include/parsec/pars/Lexer.hpp"
		"include/parsec/pars/Parser.hpp"

		"include/parsec/pars/ast.hpp"
		"include/parsec/pars/ast/Node.hpp"
		"include/parsec/pars/ast/NodeVisitor.hpp"
		"include/parsec/pars/ast/UnaryRule.hpp"
		"include/parsec/pars/ast/BinaryRule.hpp"
		"include/parsec/pars/ast/EmptyNode.hpp"
		"include/parsec/pars/ast/ListNode.hpp"
		"include/parsec/pars/ast/InlineToken.hpp"
		"include/parsec/pars/ast/NamedToken.hpp"
		"include/parsec/pars/ast/EmptyRule.hpp"
		"include/parsec/pars/ast/SymbolRule.hpp"
		"include/parsec/pars/ast/NamedRule.hpp"
		"include/parsec/pars/ast/ConcatRule.hpp"
		"include/parsec/pars/ast/AlternRule.hpp"
		"include/parsec/pars/ast/OptionalRule.hpp"
		"include/parsec/pars/ast/PlusRule.hpp"
		"include/parsec/pars/ast/StarRule.hpp"

	PRIVATE
		"src/pars/Lexer.cpp"
		"src/pars/Parser.cpp"
		"src/pars/ast.cpp"
)


target_sources(parsec-lib
	PUBLIC FILE_SET HEADERS BASE_DIRS "include" FILES
		"include/parsec/fg.hpp"
		"include/parsec/fg/Symbol.hpp"
		"include/parsec/fg/RegularExpr.hpp"
		"include/parsec/fg/SymbolRule.hpp"
		"include/parsec/fg/SymbolGrammar.hpp"
		"include/parsec/fg/regex.hpp"
		"include/parsec/fg/regex/ExprNode.hpp"
		"include/parsec/fg/regex/NodeVisitor.hpp"
		"include/parsec/fg/regex/SymbolAtom.hpp"
		"include/parsec/fg/regex/BinaryExpr.hpp"
		"include/parsec/fg/regex/ConcatExpr.hpp"
		"include/parsec/fg/regex/AlternExpr.hpp"
		"include/parsec/fg/regex/UnaryExpr.hpp"
		"include/parsec/fg/regex/StarClosure.hpp"
		"include/parsec/fg/regex/PlusClosure.hpp"
		"include/parsec/fg/regex/ParseError.hpp"
		"include/parsec/fg/regex/Parser.hpp"

	PRIVATE
		"src/fg/SymbolGrammar.cpp"
		"src/fg/RegularExpr.cpp"
		"src/fg/regex.cpp"
		"src/fg/regex/Parser.cpp"
)


target_sources(parsec-lib
	PUBLIC FILE_SET HEADERS BASE_DIRS "include" FILES
		"include/parsec/src_gen.hpp"
		"include/parsec/src_gen/ConfigStore.hpp"
		"include/parsec/src_gen/CodeGen.hpp"
		"include/parsec/src_gen/CppCodeGen.hpp"
		"include/parsec/src_gen/cpp_utils.hpp"

	PRIVATE
		"src/src_gen/CodeGen.cpp"
		"src/src_gen/CppCodeGen.cpp"
		"src/src_gen/cpp_utils.cpp"
)


target_sources(parsec-lib
	PUBLIC FILE_SET HEADERS BASE_DIRS "include" FILES
		"include/parsec/dfa/StateTrans.hpp"
		"include/parsec/dfa/State.hpp"
		"include/parsec/dfa/Automaton.hpp"
		"include/parsec/dfa/print.hpp"
		"include/parsec/dfa.hpp"

	PRIVATE
		"src/dfa/Automaton.cpp"
		"src/dfa/print.cpp"
)


target_sources(parsec-lib
	PUBLIC FILE_SET HEADERS BASE_DIRS "include" FILES
		"include/parsec/elr/StateTrans.hpp"
		"include/parsec/elr/State.hpp"
		"include/parsec/elr/StateItem.hpp"
		"include/parsec/elr/ItemSet.hpp"
		"include/parsec/elr/ReduceAction.hpp"
		"include/parsec/elr/TransNetwork.hpp"
		"include/parsec/elr/Automaton.hpp"
		"include/parsec/elr/print.hpp"
		"include/parsec/elr/hash.hpp"
		"include/parsec/elr.hpp"

	PRIVATE
		"src/elr/ItemSet.cpp"
		"src/elr/TransNetwork.cpp"
		"src/elr/Automaton.cpp"
		"src/elr/print.cpp"
		"src/elr/hash.cpp"
)


target_sources(parsec-lib
	PUBLIC FILE_SET HEADERS BASE_DIRS "include" FILES
		"include/parsec/utils/string_utils.hpp"
		"include/parsec/utils/char_utils.hpp"

	PRIVATE
		"src/utils/string_utils.cpp"
		"src/utils/char_utils.cpp"
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
