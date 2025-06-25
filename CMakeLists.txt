cmake_minimum_required(VERSION 3.30)


# Basic project information
project(parsec LANGUAGES CXX DESCRIPTION "Simple parser generator" VERSION 1.0.0)
include(CTest)

option(BUILD_APP "Build the command line tool" ON)
option(BUILD_DOCS "Generate documentation from the source tree using Doxygen" ON)
option(BUILD_EXAMPLES "Build the examples" ON)

find_package(Boost 1.85.0 CONFIG REQUIRED)
find_package(inja 3.4.0 CONFIG REQUIRED)


# Setup the main project library
add_library(parsec-lib)


target_include_directories(parsec-lib
    PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>"
    PRIVATE "include/parsec"
)

target_link_libraries(parsec-lib
    PRIVATE Boost::boost
    PRIVATE pantor::inja
)


set_target_properties(parsec-lib PROPERTIES OUTPUT_NAME parsec)

target_compile_features(parsec-lib PUBLIC cxx_std_23)


target_sources(parsec-lib
    PUBLIC FILE_SET HEADERS BASE_DIRS "include" FILES
        "include/parsec/parsec.hpp"
)


target_sources(parsec-lib
    PUBLIC FILE_SET HEADERS BASE_DIRS "include" FILES
        "include/parsec/CodeGen.hpp"
        "include/parsec/Compiler.hpp"
        "include/parsec/CompileError.hpp"

    PRIVATE
        "src/Compiler.cpp"
        "src/CompileError.cpp"
        "src/CodeGen.cpp"
)


target_sources(parsec-lib
    PUBLIC FILE_SET HEADERS BASE_DIRS "include" FILES
        "include/parsec/bnf/Symbol.hpp"
        "include/parsec/bnf/RegularExpr.hpp"
        "include/parsec/bnf/SymbolGrammar.hpp"

    PRIVATE
        "src/bnf/RegularExpr.cpp"
)


target_sources(parsec-lib
    PUBLIC FILE_SET HEADERS BASE_DIRS "include" FILES
        "include/parsec/scan/TextScanner.hpp"
        "include/parsec/scan/SourceLoc.hpp"
        "include/parsec/scan/UnexpectedEofError.hpp"

    PRIVATE
        "src/scan/TextScanner.cpp"
)


target_sources(parsec-lib
    PUBLIC FILE_SET HEADERS BASE_DIRS "include" FILES
        "include/parsec/regex/Parser.hpp"
        "include/parsec/regex/ParseError.hpp"
        "include/parsec/regex/make.hpp"
        "include/parsec/regex/ast/NodeVisitor.hpp"
        "include/parsec/regex/ast/ExprNode.hpp"
        "include/parsec/regex/ast/AtomExprNode.hpp"
        "include/parsec/regex/ast/BinaryExprNode.hpp"
        "include/parsec/regex/ast/ConcatExprNode.hpp"
        "include/parsec/regex/ast/AlternExprNode.hpp"
        "include/parsec/regex/ast/UnaryExprNode.hpp"
        "include/parsec/regex/ast/OptionalExprNode.hpp"
        "include/parsec/regex/ast/StarClosureNode.hpp"
        "include/parsec/regex/ast/PlusClosureNode.hpp"

    PRIVATE
        "src/regex/Parser.cpp"
        "src/regex/ast.cpp"
        "src/regex/ParseError.cpp"
)


target_sources(parsec-lib
    PUBLIC FILE_SET CXX_MODULES FILES
        "src/text/text.ixx"
        "src/text/chars.ixx"

        "src/fsm/fsm.ixx"
        "src/fsm/DfaStateGen.ixx"
        "src/fsm/ElrStateGen.ixx"
        "src/fsm/NameConflictError.ixx"

        "src/pars/pars.ixx"
        "src/pars/ParseError.ixx"
        "src/pars/TokenKinds.ixx"
        "src/pars/Token.ixx"
        "src/pars/Lexer.ixx"
        "src/pars/Parser.ixx"

        "src/pars/ast/ast.ixx"
        "src/pars/ast/Node.ixx"
        "src/pars/ast/NodeVisitor.ixx"
        "src/pars/ast/UnaryRuleNode.ixx"
        "src/pars/ast/BinaryRuleNode.ixx"
        "src/pars/ast/EmptyNode.ixx"
        "src/pars/ast/ListNode.ixx"
        "src/pars/ast/InlineTokenNode.ixx"
        "src/pars/ast/NamedTokenNode.ixx"
        "src/pars/ast/EmptyRuleNode.ixx"
        "src/pars/ast/SymbolRuleNode.ixx"
        "src/pars/ast/NamedRuleNode.ixx"
        "src/pars/ast/ConcatRuleNode.ixx"
        "src/pars/ast/AlternRuleNode.ixx"
        "src/pars/ast/OptionalRuleNode.ixx"
        "src/pars/ast/PlusRuleNode.ixx"
        "src/pars/ast/StarRuleNode.ixx"

    PRIVATE
        "src/fsm/DfaStateGen.cxx"
        "src/fsm/ElrStateGen.cxx"

        "src/pars/Lexer.cxx"
        "src/pars/Parser.cxx"
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
