#include <parsec/util/char_util.hpp>

#include <gtest/gtest.h>

#include <span>
#include <string>

using namespace parsec;

namespace {
	class CharUtilTest : public testing::Test {
	protected:

		template <typename T>
		void testCharConvFunc(T (*f)(char), std::span<const std::pair<std::type_identity_t<T>, char>> cases) {
			for(const auto& c : cases) {
				EXPECT_EQ(c.first, f(c.second));
			}
		}
	};


	TEST_F(CharUtilTest, charToIntConv) {
		static const std::pair<int, char> cases[] = {
			{   0,   '\0' },
			{ 128, '\x80' },
			{ 255, '\xff' }
		};
		testCharConvFunc(char_util::toInt, cases);
	}


	TEST_F(CharUtilTest, digitToIntConv) {
		static const std::pair<int, char> cases[] = {
			{ 0, '0' },
			{ 5, '5' },
			{ 9, '9' }
		};
		testCharConvFunc(char_util::evalDigit, cases);
	}


	TEST_F(CharUtilTest, hexDigitToIntConv) {
		static const std::pair<int, char> cases[] = {
			{  0, '0' },
            {  9, '9' },
            { 10, 'A' },
            { 10, 'a' },
            { 12, 'C' },
            { 12, 'c' },
            { 15, 'F' },
            { 15, 'f' }
		};
		testCharConvFunc(char_util::evalHexDigit, cases);
	}


	TEST_F(CharUtilTest, charEscape) {
		static const std::pair<std::string, char> cases[] = {
			{   R"(\0)",   '\0' },
            {   R"(\n)",   '\n' },
            {   R"(\t)",   '\t' },
            {   R"(\')",   '\'' },
			{   R"(\")",   '\"' },
            {    R"(a)",    'a' },
            { R"(\x81)", '\x81' }
		};
		testCharConvFunc(char_util::escape, cases);
	}
}
