#include <catch2/catch_test_macros.hpp>

import parsec.text;

using namespace parsec::text;


namespace {
    constexpr auto Tags = "[text]";
}


TEST_CASE("construction of single word PascalCase identifiers", Tags) {
    CHECK(toPascalCase("ab") == "Ab");
    CHECK(toPascalCase("Ab") == "Ab");
    CHECK(toPascalCase("AB") == "Ab");
}

TEST_CASE("construction of multi-word PascalCase identifiers", Tags) {
    CHECK(toPascalCase("AbAb") == "AbAb");
    CHECK(toPascalCase("abAb") == "AbAb");
}

TEST_CASE("construction of PascalCase identifiers removes non-alphanumerics", Tags) {
    CHECK(toPascalCase("ab-ab") == "AbAb");
}

TEST_CASE("construction of PascalCase identifiers removes leading digits", Tags) {
    CHECK(toPascalCase("12abAb3") == "AbAb3");
    CHECK(toPascalCase("0ab") == "Ab");
}


TEST_CASE("capitalizing a string with leading uppercase character", Tags) {
    CHECK(capitalize("A") == "A");
    CHECK(capitalize("AB") == "Ab");
    CHECK(capitalize("Ab") == "Ab");
}

TEST_CASE("capitalizing a string with leading lowercase character", Tags) {
    CHECK(capitalize("a") == "A");
    CHECK(capitalize("aB") == "Ab");
    CHECK(capitalize("ab") == "Ab");
}


TEST_CASE("converting a string to lowercase", Tags) {
    CHECK(toLower("AbC") == "abc");
}

TEST_CASE("converting a string to uppercase", Tags) {
    CHECK(toUpper("abC") == "ABC");
}

TEST_CASE("escaping a string", Tags) {
    CHECK(escape("\n\t \xffp") == "\\n\\t \\xffp");
}


TEST_CASE("character to integer conversion", Tags) {
    CHECK(toInt('\n') == 10);
    CHECK(toInt('\xff') == 255);
}

TEST_CASE("digit character evaluation", Tags) {
    CHECK(evalDigit('0') == 0);
    CHECK(evalDigit('9') == 9);
}

TEST_CASE("hexadecimal digit character evaluation", Tags) {
    CHECK(evalHexDigit('0') == 0);
    CHECK(evalHexDigit('A') == 10);
    CHECK(evalHexDigit('a') == 10);
    CHECK(evalHexDigit('f') == 15);
    CHECK(evalHexDigit('F') == 15);
}

TEST_CASE("escaping a character", Tags) {
    CHECK(escape('a') == "a");
    CHECK(escape('\n') == "\\n");
    CHECK(escape('\x03') == "\\x03");
}
