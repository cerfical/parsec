add_executable(calc "main.cpp" "ExprParser.hpp")

target_link_Libraries(calc
	PRIVATE parsec-lib
	PRIVATE Microsoft.GSL::GSL
)

target_include_directories(calc PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

add_custom_command(
	OUTPUT "ExprParser.hpp"
	COMMAND parsec ARGS "${CMAKE_CURRENT_SOURCE_DIR}/ExprParser.txt"
	MAIN_DEPENDENCY "ExprParser.txt"
)
