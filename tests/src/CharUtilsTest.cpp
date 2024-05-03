#include <parsec/utils/char_utils.hpp>

#include <gtest/gtest.h>

#include <span>
#include <string>

using namespace parsec;

namespace {
	class CharUtilsTest : public testing::Test {
	protected:
		template <typename T>
		void testCharConvFunc(T(*f)(char),
			std::span<const std::pair<std::type_identity_t<T>, char>> cases
		) {
			for(const auto& c : cases) {
				ASSERT_EQ(c.first, f(c.second));
			}
		}
	};


	TEST_F(CharUtilsTest, charToIntConv) {
		static const std::pair<int, char> cases[] = {
			{ 0, '\0' },
			{ 128, '\x80' },
			{ 255, '\xff' }
		};
		testCharConvFunc(char_utils::toInt, cases);
	}


	TEST_F(CharUtilsTest, digitToIntConv) {
		static const std::pair<int, char> cases[] = {
			{ 0, '0' },
			{ 5, '5' },
			{ 9, '9' }
		};
		testCharConvFunc(char_utils::evalDigit, cases);
	}


	TEST_F(CharUtilsTest, hexDigitToIntConv) {
		static const std::pair<int, char> cases[] = {
			{ 0, '0'},
			{ 9, '9'},
			{ 10, 'A' },
			{ 10, 'a' },
			{ 12, 'C' },
			{ 12, 'c' },
			{ 15, 'F' },
			{ 15, 'f' }
		};
		testCharConvFunc(char_utils::evalHexDigit, cases);
	}


	TEST_F(CharUtilsTest, charEscape) {
		static const std::pair<std::string, char> cases[] = {
			{ R"(\0)", '\0' },
			{ R"(\n)", '\n' },
			{ R"(\t)", '\t' },
			{ R"(\')", '\'' },
			{ R"(\")", '\"' },
			{ R"(a)", 'a' },
			{ R"(\x81)", '\x81' }
		};
		testCharConvFunc(char_utils::escape, cases);
	}
}