add_executable(calc
    "main.cxx"
    "ExprParser.hpp"
)


target_link_Libraries(calc PRIVATE parsec-lib)

target_include_directories(calc PRIVATE "${CMAKE_CURRENT_BINARY_DIR}")


add_custom_command(
    OUTPUT "ExprParser.hpp"
    COMMAND parsec
        "${CMAKE_CURRENT_SOURCE_DIR}/ExprParser.txt"
        "${CMAKE_CURRENT_BINARY_DIR}/ExprParser.hpp"
        "-t" "hpp"
        "--template-dir" "${CMAKE_SOURCE_DIR}/templates/"
    MAIN_DEPENDENCY "ExprParser.txt"
    VERBATIM
)
