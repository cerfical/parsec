find_package(Catch2 3.7.0 CONFIG)

if(NOT Catch2_FOUND)
    message(WARNING "Catch2 not found: tests disabled")
    return()
endif()


add_executable(parsec-tests
    "char_util.cpp"
    "regex_parse.cpp"
    "string_util.cpp"
)

target_link_libraries(parsec-tests
    PRIVATE parsec-lib
    PRIVATE Catch2::Catch2WithMain
)


include(Catch)

catch_discover_tests(parsec-tests)
