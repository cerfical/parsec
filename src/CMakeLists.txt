
configure_file("config.hpp.in" "config.hpp")



add_library(parsec-lib)

set_target_properties(parsec-lib
	PROPERTIES OUTPUT_NAME parsec
)

target_include_directories(parsec-lib
	PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>"
	PRIVATE "../include/parsec/"
)

target_compile_features(parsec-lib PUBLIC cxx_std_20)

target_link_libraries(parsec-lib
	PUBLIC Microsoft.GSL::GSL
	PRIVATE Boost::boost
)



target_sources(parsec-lib
	PUBLIC FILE_SET HEADERS BASE_DIRS "../include/" FILES
		"../include/parsec/parsec.hpp"
		"../include/parsec/deps.hpp"
)



target_sources(parsec-lib
	PUBLIC FILE_SET HEADERS BASE_DIRS "../include/" FILES
		"../include/parsec/core/Rule.hpp"
		"../include/parsec/core/Pattern.hpp"
		"../include/parsec/core/Symbol.hpp"
		"../include/parsec/core/CharAtom.hpp"
		"../include/parsec/core/Grammar.hpp"
	
		"../include/parsec/core/TextScanner.hpp"
		"../include/parsec/core/SourceLoc.hpp"
		"../include/parsec/core/Error.hpp"
		"../include/parsec/core/TokenKinds.hpp"
		"../include/parsec/core/TokenBase.hpp"
		"../include/parsec/core/Token.hpp"
		"../include/parsec/core/LexerBase.hpp"
		"../include/parsec/core/Lexer.hpp"
	
		"../include/parsec/core/Parser.hpp"
		"../include/parsec/core/CppSrcGen.hpp"
	
	PRIVATE
		"core/Pattern.cpp"
		"core/Grammar.cpp"
	
		"core/TextScanner.cpp"
		"core/SourceLoc.cpp"
		"core/TokenKinds.cpp"
		"core/Token.cpp"
		"core/Lexer.cpp"
	
		"core/Parser.cpp"
		"core/CppSrcGen.cpp"
)



target_sources(parsec-lib
	PUBLIC FILE_SET HEADERS BASE_DIRS "../include/" FILES
		"../include/parsec/lr/Transition.hpp"
		"../include/parsec/lr/StateReduce.hpp"
		"../include/parsec/lr/StateGen.hpp"
		"../include/parsec/lr/State.hpp"
	
	PRIVATE
		"lr/StateGen.cpp"
)



target_sources(parsec-lib
	PUBLIC FILE_SET HEADERS BASE_DIRS "../include/" FILES
		"../include/parsec/dfa/Transition.hpp"
		"../include/parsec/dfa/StateGen.hpp"
		"../include/parsec/dfa/State.hpp"

	PRIVATE
		"dfa/StateGen.cpp"
)



target_sources(parsec-lib
	PUBLIC FILE_SET HEADERS BASE_DIRS "../include/" FILES
		"../include/parsec/utils/Strings.hpp"
		"../include/parsec/utils/Chars.hpp"

	PRIVATE
		"utils/Strings.cpp"
		"utils/Chars.cpp"
)



target_sources(parsec-lib
	PUBLIC FILE_SET HEADERS BASE_DIRS "../include/" FILES
		"../include/parsec/regex/nodes.hpp"
	
		"../include/parsec/regex/ExprNode.hpp"
		"../include/parsec/regex/NodeVisitor.hpp"
		"../include/parsec/regex/ExprTraverser.hpp"
		"../include/parsec/regex/ExprPrinter.hpp"

		"../include/parsec/regex/CharAtom.hpp"
		"../include/parsec/regex/NilExpr.hpp"

		"../include/parsec/regex/BinaryExpr.hpp"
		"../include/parsec/regex/AlternExpr.hpp"
		"../include/parsec/regex/ConcatExpr.hpp"

		"../include/parsec/regex/UnaryExpr.hpp"
		"../include/parsec/regex/PlusExpr.hpp"
		"../include/parsec/regex/OptionalExpr.hpp"
		"../include/parsec/regex/StarExpr.hpp"

		"../include/parsec/regex/Parser.hpp"
	
	PRIVATE
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
)
