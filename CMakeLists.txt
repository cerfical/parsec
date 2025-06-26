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

configure_file("src/config.ixx.in" "src/config.ixx")


target_sources(parsec-lib
    PUBLIC FILE_SET CXX_MODULES FILES
        "${CMAKE_CURRENT_BINARY_DIR}/src/config.ixx"
    
        "src/parsec.ixx"
        "src/CodeGen.ixx"
        "src/Compiler.ixx"
        "src/CompileError.ixx"

        "src/text/text.ixx"
        "src/text/chars.ixx"
        "src/text/strings.ixx"

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

        "src/regex/regex.ixx"
        "src/regex/Parser.ixx"
        "src/regex/ParseError.ixx"

        "src/regex/ast/ast.ixx"
        "src/regex/ast/NodeVisitor.ixx"
        "src/regex/ast/ExprNode.ixx"
        "src/regex/ast/AtomExprNode.ixx"
        "src/regex/ast/BinaryExprNode.ixx"
        "src/regex/ast/ConcatExprNode.ixx"
        "src/regex/ast/AlternExprNode.ixx"
        "src/regex/ast/UnaryExprNode.ixx"
        "src/regex/ast/OptionalExprNode.ixx"
        "src/regex/ast/StarClosureNode.ixx"
        "src/regex/ast/PlusClosureNode.ixx"

        "src/scan/scan.ixx"
        "src/scan/TextScanner.ixx"
        "src/scan/SourceLoc.ixx"
        "src/scan/UnexpectedEofError.ixx"

        "src/bnf/bnf.ixx"
        "src/bnf/Symbol.ixx"
        "src/bnf/RegularExpr.ixx"
        "src/bnf/SymbolGrammar.ixx"

    PRIVATE
        "src/fsm/DfaStateGen.cxx"
        "src/fsm/ElrStateGen.cxx"
        
        "src/pars/Lexer.cxx"
        "src/pars/Parser.cxx"
        "src/pars/ast/ast.cxx"
        
        "src/regex/Parser.cxx"
        "src/regex/ast/ast.cxx"
        
        "src/scan/TextScanner.cxx"
        
        "src/bnf/RegularExpr.cxx"

        "src/text/text.cxx"

        "src/Compiler.cxx"
        "src/CodeGen.cxx"
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
