set(TEST_TOOL GTest)

find_package(${TEST_TOOL})


if(NOT ${TEST_TOOL}_FOUND)
    message(WARNING "${TEST_TOOL} not found: tests disabled")
    return()
endif()


include(GoogleTest)


add_executable(parsec-tests
    "src/CharUtilsTest.cpp"
    "src/StringUtilsTest.cpp"
    "src/RegularExprParseTest.cpp"
)

target_link_libraries(parsec-tests
    PRIVATE parsec-lib

    PRIVATE GTest::gtest_main
    PRIVATE GTest::gtest
    PRIVATE GTest::gmock
)


gtest_discover_tests(parsec-tests)
