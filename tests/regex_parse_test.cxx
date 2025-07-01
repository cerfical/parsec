#include <catch2/catch_test_macros.hpp>

#include <sstream>
#include <string_view>

import parsec.regex;

using namespace parsec::regex;


namespace {
    constexpr auto Tags = "[regex][parse]";

    std::string parseRegex(std::string_view str) {
        std::ostringstream out;
        out << *Parser::parseFrom(str);
        return std::move(out).str();
    }
}


TEST_CASE("parsing non-metacharacters in regular expressions", Tags) {
    CHECK(parseRegex("a") == "'a'");
}

TEST_CASE("evaluating escape sequences in regular expressions", Tags) {
    CHECK(parseRegex("\\n") == "'\\n'");
}

TEST_CASE("evaluating hexadecimal escape sequences in regular expressions", Tags) {
    CHECK(parseRegex("\\xff") == "'\\xff'");
}

TEST_CASE("parsing empty regular expressions", Tags) {
    CHECK(parseRegex("") == "()");
    CHECK(parseRegex("()") == "()");
}


TEST_CASE("parsing empty character sets", Tags) {
    CHECK(parseRegex("[]") == "()");
}

TEST_CASE("parsing character sets with a single element", Tags) {
    CHECK(parseRegex("[a]") == "'a'");
}

TEST_CASE("parsing character sets with multiple elements", Tags) {
    CHECK(parseRegex("[ab]") == "('a' | 'b')");
    CHECK(parseRegex("[abc]") == "(('a' | 'b') | 'c')");
}

TEST_CASE("parsing character sets with character ranges", Tags) {
    CHECK(parseRegex("[-da-co-op]") == "(((('-' | 'd') | (('a' | 'b') | 'c')) | 'o') | 'p')");
}


TEST_CASE("parsing character ranges with a single element", Tags) {
    CHECK(parseRegex("[a-a]") == "'a'");
}

TEST_CASE("parsing character ranges with no low or high bound", Tags) {
    CHECK(parseRegex("[a-]") == "('a' | '-')");
    CHECK(parseRegex("[-a]") == "('-' | 'a')");
    CHECK(parseRegex("[--]") == "('-' | '-')");
    CHECK(parseRegex("[a-a-b]") == "(('a' | '-') | 'b')");
}

TEST_CASE("parsing character ranges with multiple elements", Tags) {
    CHECK(parseRegex("[a-b]") == "('a' | 'b')");
    CHECK(parseRegex("[a-c]") == "(('a' | 'b') | 'c')");
}


TEST_CASE("parsing character concatenations", Tags) {
    CHECK(parseRegex("ab") == "('a' 'b')");
    CHECK(parseRegex("help") == "((('h' 'e') 'l') 'p')");
}

TEST_CASE("parsing character alternations", Tags) {
    CHECK(parseRegex("a|b") == "('a' | 'b')");
    CHECK(parseRegex("a|b|c") == "(('a' | 'b') | 'c')");
}

TEST_CASE("parsing 'zero or one' applied to a character", Tags) {
    CHECK(parseRegex("a?") == "'a'?");
}

TEST_CASE("parsing 'zero or more' applied to a character", Tags) {
    CHECK(parseRegex("a*") == "'a'*");
}

TEST_CASE("parsing 'one or more' applied to a character", Tags) {
    CHECK(parseRegex("a+") == "'a'+");
}


TEST_CASE("parsing concatenations of alternation expressions", Tags) {
    CHECK(parseRegex("(a|b)(c|d)") == "(('a' | 'b') ('c' | 'd'))");
}

TEST_CASE("parsing alternations of concatenation expressions", Tags) {
    CHECK(parseRegex("ab|cd") == "(('a' 'b') | ('c' 'd'))");
}
