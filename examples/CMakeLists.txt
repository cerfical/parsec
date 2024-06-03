add_executable(calc
    "calc/main.cpp"
    "ExprParser.hpp"
)


target_link_Libraries(calc PRIVATE parsec-lib)

target_include_directories(calc PRIVATE "${CMAKE_CURRENT_BINARY_DIR}")


add_custom_command(
    OUTPUT "ExprParser.hpp"
    COMMAND parsec-app "${CMAKE_CURRENT_SOURCE_DIR}/calc/ExprParser.txt" "${CMAKE_CURRENT_BINARY_DIR}/ExprParser.hpp"
    MAIN_DEPENDENCY "calc/ExprParser.txt"
    VERBATIM
)
