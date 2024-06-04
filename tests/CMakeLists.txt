set(TEST_TOOL Catch2)

find_package(${TEST_TOOL})


if(NOT ${TEST_TOOL}_FOUND)
    message(WARNING "${TEST_TOOL} not found: tests disabled")
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
