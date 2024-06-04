#include <parsec/util/string_util.hpp>

#include <catch2/catch_test_macros.hpp>


using namespace parsec::string_util;

constexpr auto Tags = "[string_util][util]";


SCENARIO("construction of PascalCase identifiers", Tags) {
    GIVEN("a single word identifier") {
        THEN("capitalize the word") {
            CHECK(toPascalCase("ab") == "Ab");
            CHECK(toPascalCase("Ab") == "Ab");
            CHECK(toPascalCase("AB") == "Ab");
        }
    }

    GIVEN("a multi-word identifier") {
        THEN("capitalize each word separately") {
            CHECK(toPascalCase("AbAb") == "AbAb");
            CHECK(toPascalCase("abAb") == "AbAb");
        }
    }

    GIVEN("non-alphanumeric characters") {
        THEN("remove the characters and capitalize the next word") {
            CHECK(toPascalCase("ab-ab") == "AbAb");
        }
    }

    GIVEN("a first word with leading decimal digits") {
        THEN("remove the digits") {
            CHECK(toPascalCase("12abAb3") == "AbAb3");
            CHECK(toPascalCase("0ab") == "Ab");
        }
    }
}


SCENARIO("capitalize a string", Tags) {
    GIVEN("a single character") {
        THEN("convert the character to uppercase") {
            CHECK(capitalize("a") == "A");
            CHECK(capitalize("A") == "A");
        }
    }

    GIVEN("multiple characters") {
        THEN("make the first character uppercase and the rest lowercase") {
            CHECK(capitalize("ab") == "Ab");
            CHECK(capitalize("AB") == "Ab");
            CHECK(capitalize("Ab") == "Ab");
        }
    }
}


SCENARIO("converting a string to lowercase", Tags) {
    GIVEN("a non-empty string") {
        THEN("every character in a string is made lowercase") {
            CHECK(toLower("AbC") == "abc");
        }
    }
}


SCENARIO("converting a string to uppercase", Tags) {
    GIVEN("a non-empty string") {
        THEN("every character in a string is made uppercase") {
            CHECK(toUpper("abC") == "ABC");
        }
    }
}


SCENARIO("escaping a string", Tags) {
    GIVEN("a non-empty string") {
        THEN("every character in a string is escaped") {
            CHECK(escape("\n\t \xffp") == "\\n\\t \\xffp");
        }
    }
}
