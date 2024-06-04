#include <parsec/util/char_util.hpp>

#include <catch2/catch_test_macros.hpp>


using namespace parsec::char_util;

constexpr auto Tags = "[char_util][util]";


SCENARIO("character to integer conversion", Tags) {
    GIVEN("an ASCII or extended ASCII character") {
        THEN("return the corresponding numeric code") {
            CHECK(toInt('\n') == 10);
            CHECK(toInt('\xff') == 255);
        }
    }
}


SCENARIO("digit character evaluation", Tags) {
    GIVEN("a decimal digit") {
        THEN("return its numeric value") {
            CHECK(evalDigit('5') == 5);
        }
    }
}


SCENARIO("hexadecimal digit character evaluation", Tags) {
    GIVEN("a decimal or a lowercase/uppercase hexadecimal digit") {
        THEN("return its numeric value") {
            CHECK(evalHexDigit('7') == 7);
            CHECK(evalHexDigit('b') == 11);
            CHECK(evalHexDigit('F') == 15);
        }
    }
}


SCENARIO("escaping a character", Tags) {
    GIVEN("a printable character") {
        THEN("return the character as is") {
            CHECK(escape('a') == "a");
        }
    }

    GIVEN("a non-printable character with a common escape sequence") {
        THEN("return the sequence") {
            CHECK(escape('\n') == "\\n");
        }
    }

    GIVEN("a non-printable character") {
        THEN("return the hexadecimal escape sequence for the character") {
            CHECK(escape('\x03') == "\\x03");
        }
    }
}
