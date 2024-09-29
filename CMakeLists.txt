cmake_minimum_required(VERSION 3.23)


# Basic project information
project(parsec LANGUAGES CXX DESCRIPTION "Simple parser generator" VERSION 1.0.0)
include(CTest)

option(BUILD_APP "Build the command line tool" ON)
option(BUILD_DOCS "Generate documentation from the source tree using Doxygen" ON)
option(BUILD_EXAMPLES "Build the examples" ON)

find_package(Boost REQUIRED)


# Setup the main project library
add_library(parsec-lib)


target_include_directories(parsec-lib
    PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>"
    PRIVATE "include/parsec"
)

target_link_libraries(parsec-lib
    PRIVATE Boost::boost
)


set_target_properties(parsec-lib PROPERTIES OUTPUT_NAME parsec)

target_compile_features(parsec-lib PUBLIC cxx_std_23)


# List all source and header files for the library
target_sources(parsec-lib
    PUBLIC FILE_SET HEADERS BASE_DIRS "include" FILES
        "include/parsec/parsec.hpp"
        "include/parsec/deps.hpp"
        "include/parsec/print.hpp"

    PRIVATE
        "src/print.cpp"
)


target_sources(parsec-lib
    PUBLIC FILE_SET HEADERS BASE_DIRS "include" FILES
        "include/parsec/core/TextScanner.hpp"
        "include/parsec/core/SourceLoc.hpp"
        "include/parsec/core/Compiler.hpp"
        "include/parsec/core/Error.hpp"
        "include/parsec/core/NameConflictError.hpp"
        "include/parsec/core/ParseError.hpp"
        "include/parsec/core/Symbol.hpp"
        "include/parsec/core/RegularExpr.hpp"
        "include/parsec/core/SymbolRule.hpp"
        "include/parsec/core/SymbolGrammar.hpp"

    PRIVATE
        "src/core/TextScanner.cpp"
        "src/core/Compiler.cpp"
        "src/core/ParseError.cpp"
        "src/core/SymbolGrammar.cpp"
        "src/core/RegularExpr.cpp"
)


target_sources(parsec-lib
    PUBLIC FILE_SET HEADERS BASE_DIRS "include" FILES
        "include/parsec/pars/TokenKinds.hpp"
        "include/parsec/pars/Token.hpp"
        "include/parsec/pars/Lexer.hpp"
        "include/parsec/pars/Parser.hpp"
        "include/parsec/pars/ast/Node.hpp"
        "include/parsec/pars/ast/NodeVisitor.hpp"
        "include/parsec/pars/ast/UnaryRule.hpp"
        "include/parsec/pars/ast/BinaryRule.hpp"
        "include/parsec/pars/ast/EmptyNode.hpp"
        "include/parsec/pars/ast/ListNode.hpp"
        "include/parsec/pars/ast/InlineToken.hpp"
        "include/parsec/pars/ast/NamedToken.hpp"
        "include/parsec/pars/ast/EmptyRule.hpp"
        "include/parsec/pars/ast/SymbolAtom.hpp"
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
        "include/parsec/regex/Parser.hpp"
        "include/parsec/regex/make.hpp"
        "include/parsec/regex/nodes/NodeVisitor.hpp"
        "include/parsec/regex/nodes/ExprNode.hpp"
        "include/parsec/regex/nodes/SymbolAtom.hpp"
        "include/parsec/regex/nodes/BinaryExpr.hpp"
        "include/parsec/regex/nodes/ConcatExpr.hpp"
        "include/parsec/regex/nodes/AlternExpr.hpp"
        "include/parsec/regex/nodes/UnaryExpr.hpp"
        "include/parsec/regex/nodes/StarClosure.hpp"
        "include/parsec/regex/nodes/PlusClosure.hpp"

    PRIVATE
        "src/regex/Parser.cpp"
        "src/regex/nodes.cpp"
        "src/regex/make.cpp"
)


target_sources(parsec-lib
    PUBLIC FILE_SET HEADERS BASE_DIRS "include" FILES
        "include/parsec/src_gen/ConfigStore.hpp"
        "include/parsec/src_gen/CodeGen.hpp"
        "include/parsec/src_gen/CppCodeGen.hpp"

    PRIVATE
        "src/src_gen/CppCodeGen.cpp"
)


target_sources(parsec-lib
    PUBLIC FILE_SET HEADERS BASE_DIRS "include" FILES
        "include/parsec/dfa/State.hpp"
        "include/parsec/dfa/Automaton.hpp"

    PRIVATE
        "src/dfa/Automaton.cpp"
)


target_sources(parsec-lib
    PUBLIC FILE_SET HEADERS BASE_DIRS "include" FILES
        "include/parsec/elr/State.hpp"
        "include/parsec/elr/Automaton.hpp"

    PRIVATE
        "src/elr/Automaton.cpp"
)


target_sources(parsec-lib
    PUBLIC FILE_SET HEADERS BASE_DIRS "include" FILES
        "include/parsec/util/string_util.hpp"
        "include/parsec/util/char_util.hpp"
        "include/parsec/util/cpp_util.hpp"

    PRIVATE
        "src/util/string_util.cpp"
        "src/util/char_util.cpp"
        "src/util/cpp_util.cpp"
)


# Optional components
if(BUILD_APP)
    add_subdirectory(app)
endif()

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()

if(BUILD_DOCS)
    add_subdirectory(docs)
endif()

if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()


# Generate .gitignore file to take care of the build directory automatically
file(WRITE "${CMAKE_BINARY_DIR}/.gitignore" [[
*
]])

configure_file("src/config.hpp.in" "include/parsec/config.hpp")
