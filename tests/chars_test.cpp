#include <catch2/catch_test_macros.hpp>
#include <parsec/text/chars.hpp>

using namespace parsec::text::chars;


namespace {
    constexpr auto Tags = "[text][chars]";
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
