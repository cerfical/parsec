#include <catch2/catch_test_macros.hpp>
#include <parsec/regex/Parser.hpp>

#include <sstream>
#include <string_view>

using namespace parsec::regex;


namespace {
    constexpr auto Tags = "[regex][parse]";

    std::string parseRegex(std::string_view str) {
        std::ostringstream out;
        out << *Parser::parseFrom(str);
        return std::move(out).str();
    }
}


TEST_CASE("non-metacharacter atoms are taken literally", Tags) {
    CHECK(parseRegex("a") == "'a'");
}

TEST_CASE("evaluating escape sequences", Tags) {
    CHECK(parseRegex("\\n") == "'\\n'");
}

TEST_CASE("evaluating hexadecimal escape sequences", Tags) {
    CHECK(parseRegex("\\xff") == "'\\xff'");
}

TEST_CASE("empty expressions", Tags) {
    CHECK(parseRegex("") == "()");
    CHECK(parseRegex("()") == "()");
}


TEST_CASE("empty character set is an empty expression", Tags) {
    CHECK(parseRegex("[]") == "()");
}

TEST_CASE("character set with a single element", Tags) {
    CHECK(parseRegex("[a]") == "'a'");
}

TEST_CASE("character set with multiple elements", Tags) {
    CHECK(parseRegex("[ab]") == "('a' | 'b')");
    CHECK(parseRegex("[abc]") == "(('a' | 'b') | 'c')");
}

TEST_CASE("character set with character ranges", Tags) {
    CHECK(parseRegex("[-da-co-op]") == "(((('-' | 'd') | (('a' | 'b') | 'c')) | 'o') | 'p')");
}


TEST_CASE("character range with a single element", Tags) {
    CHECK(parseRegex("[a-a]") == "'a'");
}

TEST_CASE("character range with no low or high bound", Tags) {
    CHECK(parseRegex("[a-]") == "('a' | '-')");
    CHECK(parseRegex("[-a]") == "('-' | 'a')");
    CHECK(parseRegex("[--]") == "('-' | '-')");
    CHECK(parseRegex("[a-a-b]") == "(('a' | '-') | 'b')");
}

TEST_CASE("character range with multiple elements", Tags) {
    CHECK(parseRegex("[a-b]") == "('a' | 'b')");
    CHECK(parseRegex("[a-c]") == "(('a' | 'b') | 'c')");
}


TEST_CASE("character concatenation", Tags) {
    CHECK(parseRegex("ab") == "('a' 'b')");
    CHECK(parseRegex("help") == "((('h' 'e') 'l') 'p')");
}

TEST_CASE("character alternation", Tags) {
    CHECK(parseRegex("a|b") == "('a' | 'b')");
    CHECK(parseRegex("a|b|c") == "(('a' | 'b') | 'c')");
}

TEST_CASE("zero or one character", Tags) {
    CHECK(parseRegex("a?") == "'a'?");
}

TEST_CASE("zero or more characters", Tags) {
    CHECK(parseRegex("a*") == "'a'*");
}

TEST_CASE("one or more characters", Tags) {
    CHECK(parseRegex("a+") == "'a'+");
}


TEST_CASE("concatenation of alternation expressions", Tags) {
    CHECK(parseRegex("(a|b)(c|d)") == "(('a' | 'b') ('c' | 'd'))");
}

TEST_CASE("alternation of concatenation expressions", Tags) {
    CHECK(parseRegex("ab|cd") == "(('a' 'b') | ('c' 'd'))");
}
