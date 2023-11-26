
# Finds and adds to project files all sources and headers
function(collect_module_sources TARGET MODULE)
	# Prefix all files with the name of the containing module
	if(MODULE)
		set(FILE_PREFIX "${MODULE}/")
	else()
		set(FILE_PREFIX "")
	endif()

	foreach(NAME ${ARGN})
		# Add header files to the target's public headers
		set(HEADER_FILE "${CMAKE_SOURCE_DIR}/include/parsec/${FILE_PREFIX}${NAME}.hpp")
		if(EXISTS ${HEADER_FILE})
			target_sources(${TARGET} PUBLIC FILE_SET HEADERS FILES ${HEADER_FILE} BASE_DIRS "../include/")
		endif()

		# Add header files to the target's private source files
		set(SOURCE_FILE "${CMAKE_SOURCE_DIR}/src/${FILE_PREFIX}${NAME}.cpp")
		if(EXISTS ${SOURCE_FILE})
			target_sources(${TARGET} PRIVATE ${SOURCE_FILE})
		endif()

		if(NOT EXISTS ${HEADER_FILE} AND NOT EXISTS ${SOURCE_FILE})
			message(SEND_ERROR "Neither ${HEADER_FILE} or ${SOURCE_FILE} were found")
		endif()
	endforeach()
endfunction()


# Write project information to a header file
configure_file("config.hpp.in" "config.hpp")


add_library(parsec-lib)

set_target_properties(parsec-lib
	PROPERTIES OUTPUT_NAME "${PROJECT_NAME}"
)


collect_module_sources(parsec-lib "" "parsec" "deps")

collect_module_sources(parsec-lib "core"
	"Rule"
	"Pattern"
	"Symbol"
	"CharAtom"
	"Grammar"
	
	"TextScanner"
	"SourceLoc"
	"Error"
	"TokenKinds"
	"TokenBase"
	"Token"
	"LexerBase"
	"Lexer"
	
	"Parser"
	"CppSrcGen"
)


collect_module_sources(parsec-lib "regex"
	"nodes"
	
	"ExprNode"
	"NodeVisitor"
	"ExprTraverser"
	"ExprPrinter"

	"CharAtom"
	"NilExpr"

	"BinaryExpr"
	"AlternExpr"
	"ConcatExpr"

	"UnaryExpr"
	"PlusExpr"
	"OptionalExpr"
	"StarExpr"

	"Parser"
)


collect_module_sources(parsec-lib "lr"
	"Transition"
	"StateReduce"
	"StateGen"
	"State"
)


collect_module_sources(parsec-lib "dfa"
	"Transition" "StateGen" "State"
)

collect_module_sources(parsec-lib "utils"
	"Strings" "Chars"
)


# Specify build and usage requirements
target_include_directories(parsec-lib
	PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>"
	PRIVATE "../include/parsec/"
)

target_compile_features(parsec-lib PUBLIC cxx_std_20)
target_link_libraries(parsec-lib
	PUBLIC Microsoft.GSL::GSL
	PRIVATE Boost::boost
)
