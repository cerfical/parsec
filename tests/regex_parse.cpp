#include <parsec/regex/Parser.hpp>
#include <parsec/print.hpp>

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_tostring.hpp>

#include <sstream>
#include <string_view>


using namespace parsec::regex;
using namespace parsec;

constexpr auto Tags = "[regex][parse]";


SCENARIO("parsing a regular expression", Tags) {
    const auto parseRegex = [](std::string_view str) {
        const auto ast = Parser::parse(str);
        std::ostringstream out;
        out << *ast;
        return std::move(out).str();
    };

    GIVEN("a regular expression represented as a string") {
        WHEN("the expression is a character with no special meaning") {
            THEN("construct an atom with the character") {
                CHECK(parseRegex("a") == "'a'");
            }
        }

        WHEN("the expression is an escape sequence") {
            THEN("construct an atom with a character corresponding to the sequence") {
                CHECK(parseRegex("\\n") == "'\n'");
            }
        }

        WHEN("the expression is a hexadecimal escape sequence") {
            THEN("construct an atom with a character with the specified numeric code") {
                CHECK(parseRegex("\\xff") == "'\xff'");
            }
        }

        WHEN("the expression contains multiple subexpressions") {
            THEN("combine the subexpressions by concatenation") {
                CHECK(parseRegex("ab") == "('a' 'b')");
            }
        }

        WHEN("the expression contains multiple subexpressions separated by vertical bars") {
            THEN("combine the subexpressions by alternation") {
                CHECK(parseRegex("a|b") == "('a' | 'b')");
            }
        }

        WHEN("the expression is empty or is an empty set of parentheses") {
            THEN("construct an empty expression") {
                CHECK(parseRegex("") == "()");
                CHECK(parseRegex("()") == "()");
            }
        }

        WHEN("the expression ends with a question mark") {
            THEN("construct an optional expression") {
                CHECK(parseRegex("a?") == "'a'?");
            }
        }

        WHEN("the expression ends with a plus") {
            THEN("construct a Kleene plus expression") {
                CHECK(parseRegex("a+") == "'a'+");
            }
        }

        WHEN("the expression ends with a star") {
            THEN("construct a Kleene star expression") {
                CHECK(parseRegex("a*") == "'a'*");
            }
        }

        WHEN("the expression is a character set") {
            AND_WHEN("the set is empty") {
                THEN("construct an empty expression") {
                    CHECK(parseRegex("[]") == "()");
                }
            }

            AND_WHEN("the set contains a single element") {
                THEN("construct an atom from the only element") {
                    CHECK(parseRegex("[a]") == "'a'");
                }
            }

            AND_WHEN("the set contains multiple consecutive elements") {
                THEN("combine the elements by alternation") {
                    CHECK(parseRegex("[ab]") == "('a' | 'b')");
                }
            }

            AND_WHEN("the set contains character range") {
                AND_WHEN("the range has equal lower and upper bounds") {
                    THEN("construct an atom with the single character") {
                        CHECK(parseRegex("[a-a]") == "'a'");
                    }
                }

                AND_WHEN("the range has no lower or upper bound") {
                    THEN("treat the range delimiter as a regular character") {
                        CHECK(parseRegex("[a-]") == "('a' | '-')");
                        CHECK(parseRegex("[-a]") == "('-' | 'a')");
                    }
                }

                AND_WHEN("the range has upper bound greater than lower bound") {
                    THEN("combine all characters lying in the range by alternation") {
                        CHECK(parseRegex("[a-c]") == "(('a' | 'b') | 'c')");
                    }
                }
            }
        }
    }
}
