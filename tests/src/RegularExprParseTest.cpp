#include <parsec/parsec.hpp>

#include <gtest/gtest.h>

#include <span>
#include <string>

using namespace parsec;

namespace {
	class RegularExprParseTest : public testing::Test {
	protected:
		void testParse(std::span<const std::pair<std::string, std::string>> cases) {
			for(const auto& c : cases) {
				auto got = testing::PrintToString(*regex::Parser().parse(c.second));
				auto& expected = c.first;

				EXPECT_EQ(expected, got);
			}
		}
	};


	TEST_F(RegularExprParseTest, concatExprParse) {
		static const std::pair<std::string, std::string> cases[] = {
			{ "('a' 'b')", "ab" },
			{ "(('a' 'b') 'c')", "abc" }
		};
		testParse(cases);
	}


	TEST_F(RegularExprParseTest, alternExprParse) {
		static const std::pair<std::string, std::string> cases[] = {
			{ "('a' | 'b')", "a|b" },
			{ "(('a' | 'b') | 'c')", "a|b|c" }
		};
		testParse(cases);
	}


	TEST_F(RegularExprParseTest, atomExprParse) {
		static const std::pair<std::string, std::string> cases[] = {
			{ "'a'", "a" },
			{ "'\n'", R"(\n)"},
			{ "'\xff'", R"(\xff)" }
		};
		testParse(cases);
	}


	TEST_F(RegularExprParseTest, emptyExprParse) {
		static const std::pair<std::string, std::string> cases[] = {
			{ "()", "" },
			{ "()", "()" },
			{ "()", "[]" }
		};
		testParse(cases);
	}


	TEST_F(RegularExprParseTest, charSetParse) {
		static const std::pair<std::string, std::string> cases[] = {
			{ "'a'", "[a-a]" },
			{ "('a' | 'b')", "[ab]" },
			{ "('a' | '-')", "[a-]" },
			{ "('-' | 'a')", "[-a]" },
			{ "(('a' | 'b') | 'c')", "[a-c]" },
			{ "(('a' | 'b') | 'c')", "[a-bc]" }
		};
		testParse(cases);
	}


	TEST_F(RegularExprParseTest, optionalExprParse) {
		static const std::pair<std::string, std::string> cases[] = {
			{ "'a'?", "a?" }
		};
		testParse(cases);
	}


	TEST_F(RegularExprParseTest, starClosureParse) {
		static const std::pair<std::string, std::string> cases[] = {
			{ "'a'*", "a*" }
		};
		testParse(cases);
	}


	TEST_F(RegularExprParseTest, plusClosureParse) {
		static const std::pair<std::string, std::string> cases[] = {
			{ "'a'+", "a+" }
		};
		testParse(cases);
	}
}