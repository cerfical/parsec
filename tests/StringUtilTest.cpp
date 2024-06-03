#include <parsec/util/string_util.hpp>

#include <gtest/gtest.h>

#include <span>
#include <string>

using namespace parsec;

namespace {
	class StringUtilTest : public testing::Test {
	protected:
		void testStringConvFunc(std::string(*f)(std::string_view),
			std::span<const std::pair<std::string, std::string>> cases
		) {
			for(const auto& c : cases) {
				EXPECT_EQ(c.first, f(c.second));
			}
		}
	};


	TEST_F(StringUtilTest, toPascalCaseConv) {
		static const std::pair<std::string, std::string> cases[] = {
			{ "", "" },
			{ "A", "a" },
			{ "A", "A" },
			{ "AA", "a-a" },
			{ "Ab", "ab" },
			{ "Ab", "Ab" },
			{ "AbAb", "ab-ab" },
			{ "AbAb", "Ab-Ab" },
			{ "AbAb", "AB-AB" },
			{ "AbAb", "AbAb" },
			{ "AbAb", "abAb" },
			{ "AbAb3", "12abAb3" }
		};
		testStringConvFunc(string_util::toPascalCase, cases);
	}


	TEST_F(StringUtilTest, toCapitalCaseConv) {
		static const std::pair<std::string, std::string> cases[] = {
			{ "", "" },
			{ "A", "a" },
			{ "Ab", "ab" },
			{ "Ab", "Ab" },
			{ "Ab", "AB" }
		};
		testStringConvFunc(string_util::capitalize, cases);
	}


	TEST_F(StringUtilTest, toLowerCaseConv) {
		static const std::pair<std::string, std::string> cases[] = {
			{ "", "" },
			{ "a", "A" },
			{ "a", "a" },
			{ "ab", "Ab" },
			{ "ab", "AB" }
		};
		testStringConvFunc(string_util::toLower, cases);
	}


	TEST_F(StringUtilTest, toUpperCaseConv) {
		static const std::pair<std::string, std::string> cases[] = {
			{ "", "" },
			{ "A", "a" },
			{ "A", "A" },
			{ "AB", "Ab" },
			{ "AB", "AB" }
		};
		testStringConvFunc(string_util::toUpper, cases);
	}


	TEST_F(StringUtilTest, stringEscape) {
		static const std::pair<std::string, std::string> cases[] = {
			{ R"()", "" },
			{ R"(abc)", "abc" },
			{ R"(\n\t \xffp)", "\n\t \xffp" }
		};
		testStringConvFunc(string_util::escape, cases);
	}
}