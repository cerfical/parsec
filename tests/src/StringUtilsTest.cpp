#include <parsec/utils/string_utils.hpp>

#include <gtest/gtest.h>

#include <span>
#include <string>

using namespace parsec;

namespace {
	class StringUtilsTest : public testing::Test {
	protected:
		void testStringConvFunc(std::string(*f)(std::string_view),
			std::span<const std::pair<std::string, std::string>> cases
		) {
			for(const auto& c : cases) {
				ASSERT_EQ(c.first, f(c.second));
			}
		}
	};


	TEST_F(StringUtilsTest, toPascalCaseConv) {
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
		testStringConvFunc(string_utils::toPascalCase, cases);
	}


	TEST_F(StringUtilsTest, toCapitalCaseConv) {
		static const std::pair<std::string, std::string> cases[] = {
			{ "", "" },
			{ "A", "a" },
			{ "Ab", "ab" },
			{ "Ab", "Ab" },
			{ "Ab", "AB" }
		};
		testStringConvFunc(string_utils::capitalize, cases);
	}


	TEST_F(StringUtilsTest, toLowerCaseConv) {
		static const std::pair<std::string, std::string> cases[] = {
			{ "", "" },
			{ "a", "A" },
			{ "a", "a" },
			{ "ab", "Ab" },
			{ "ab", "AB" }
		};
		testStringConvFunc(string_utils::toLower, cases);
	}


	TEST_F(StringUtilsTest, toUpperCaseConv) {
		static const std::pair<std::string, std::string> cases[] = {
			{ "", "" },
			{ "A", "a" },
			{ "A", "A" },
			{ "AB", "Ab" },
			{ "AB", "AB" }
		};
		testStringConvFunc(string_utils::toUpper, cases);
	}


	TEST_F(StringUtilsTest, stringEscape) {
		static const std::pair<std::string, std::string> cases[] = {
			{ R"()", "" },
			{ R"(abc)", "abc" },
			{ R"(\n\t \xffp)", "\n\t \xffp" }
		};
		testStringConvFunc(string_utils::escape, cases);
	}
}