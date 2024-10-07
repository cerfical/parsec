cmake_minimum_required(VERSION 3.23)


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
        "include/parsec/core/CodeGen.hpp"
        
        PRIVATE
        "src/core/TextScanner.cpp"
        "src/core/Compiler.cpp"
        "src/core/ParseError.cpp"
        "src/core/SymbolGrammar.cpp"
        "src/core/RegularExpr.cpp"
        "src/core/CodeGen.cpp"
)


target_sources(parsec-lib
    PUBLIC FILE_SET HEADERS BASE_DIRS "include" FILES
        "include/parsec/pars/TokenKinds.hpp"
        "include/parsec/pars/Token.hpp"
        "include/parsec/pars/Lexer.hpp"
        "include/parsec/pars/Parser.hpp"
        "include/parsec/pars/ast/Node.hpp"
        "include/parsec/pars/ast/NodeVisitor.hpp"
        "include/parsec/pars/ast/UnaryRuleNode.hpp"
        "include/parsec/pars/ast/BinaryRuleNode.hpp"
        "include/parsec/pars/ast/EmptyNode.hpp"
        "include/parsec/pars/ast/ListNode.hpp"
        "include/parsec/pars/ast/InlineTokenNode.hpp"
        "include/parsec/pars/ast/NamedTokenNode.hpp"
        "include/parsec/pars/ast/EmptyRuleNode.hpp"
        "include/parsec/pars/ast/SymbolRuleNode.hpp"
        "include/parsec/pars/ast/NamedRuleNode.hpp"
        "include/parsec/pars/ast/ConcatRuleNode.hpp"
        "include/parsec/pars/ast/AlternRuleNode.hpp"
        "include/parsec/pars/ast/OptionalRuleNode.hpp"
        "include/parsec/pars/ast/PlusRuleNode.hpp"
        "include/parsec/pars/ast/StarRuleNode.hpp"

    PRIVATE
        "src/pars/Lexer.cpp"
        "src/pars/Parser.cpp"
        "src/pars/ast.cpp"
)


target_sources(parsec-lib
    PUBLIC FILE_SET HEADERS BASE_DIRS "include" FILES
        "include/parsec/regex/Parser.hpp"
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
)


target_sources(parsec-lib
    PUBLIC FILE_SET HEADERS BASE_DIRS "include" FILES
        "include/parsec/fsm/StateTrans.hpp"
        "include/parsec/fsm/DfaState.hpp"
        "include/parsec/fsm/DfaAutomaton.hpp"
        "include/parsec/fsm/ElrState.hpp"
        "include/parsec/fsm/ElrAutomaton.hpp"

    PRIVATE
        "src/fsm/DfaAutomaton.cpp"
        "src/fsm/ElrAutomaton.cpp"
)


target_sources(parsec-lib
    PUBLIC FILE_SET HEADERS BASE_DIRS "include" FILES
        "include/parsec/util/string_util.hpp"
        "include/parsec/util/chars.hpp"

    PRIVATE
        "src/util/string_util.cpp"
        "src/util/chars.cpp"
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
