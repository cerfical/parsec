#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <parsec/regex/Parser.hpp>
#include <parsec/regex/RegularGrammar.hpp>
#include <parsec/regex/nodes.hpp>

#include <parsec/core/ParseError.hpp>

#include <sstream>
#include <format>

using namespace testing;

namespace parsec::regex {
	namespace {
		using namespace nodes;

		class RegularExprTest : public Test {
		protected:
			
			std::set<char> metachars = { '(', ')', '[', ']', '|', '+', '*', '?' };



			template <typename Node, typename... Args>
			void stringifyAndCheck(std::string_view expect, Args&&... args) {
				const auto regex = makeExpr<Node>(std::forward<Args>(args)...);
				const auto regexStr = PrintToString(*regex);
				ASSERT_EQ(regexStr, expect);
			}

			template <std::derived_from<UnaryExpr> Node>
			void stringifyAndCheck(std::string_view expect, char inner) {
				stringifyAndCheck<Node>(expect, makeExpr<CharAtom>(inner));
			}

			template <std::derived_from<BinaryExpr> Node>
			void stringifyAndCheck(std::string_view expect, char left, char right) {
				stringifyAndCheck<Node>(expect, makeExpr<CharAtom>(left), makeExpr<CharAtom>(right));
			}



			void parseAndCheck(std::string regex, std::string expect) {
				const auto parsedRegexStr = PrintToString(*parse(regex));
				ASSERT_EQ(parsedRegexStr, expect);
			}

			void parseMustThrow(std::string regex) {
				ASSERT_THROW(parse(regex), ParseError);
			}

			ExprPtr parse(std::string_view regex) {
				return Parser().parse(regex);
			}
	
		};



		TEST_F(RegularExprTest, Print_NilExpr_Prints_LeftParen_RightParen) {
			stringifyAndCheck<NilExpr>("()");
			stringifyAndCheck<CharAtom>("a", 'a');
		}

		TEST_F(RegularExprTest, Print_CharAtom_Prints_AtomValue) {
			stringifyAndCheck<CharAtom>("a", 'a');
		}

		TEST_F(RegularExprTest, Print_Unary_Prints_Inner_Op) {
			stringifyAndCheck<OptionalExpr>("a?", 'a');
			stringifyAndCheck<PlusExpr>("a+", 'a');
			stringifyAndCheck<StarExpr>("a*", 'a');
		}

		TEST_F(RegularExprTest, Print_Binary_Prints_LeftParen_Left_Op_Right_RightParen) {
			stringifyAndCheck<AlternExpr>("(a|b)", 'a', 'b');
			stringifyAndCheck<ConcatExpr>("(ab)", 'a', 'b');
		}



		TEST_F(RegularExprTest, EqualNodes_CompareEqual) {
			const auto n1 = parse("a|b");
			const auto n2 = parse("a|b");
			ASSERT_EQ(*n1, *n1);
			ASSERT_EQ(*n1, *n2);
		}

		TEST_F(RegularExprTest, NotEqualNodes_CompareNotEqual) {
			const auto n1 = parse("a|b");
			const auto n2 = parse("a|c");
			ASSERT_NE(*n1, *n2);
		}



		TEST_F(RegularExprTest, Parse_Char_Creates_CharAtom) {
			parseAndCheck("a", "a");
		}

		TEST_F(RegularExprTest, Parse_EmptyParentheses_Creates_NilExpr) {
			parseAndCheck("()", "()");
		}

		TEST_F(RegularExprTest, Parse_EmptyString_Creates_NilExpr) {
			parseAndCheck("", "()");
		}



		TEST_F(RegularExprTest, Parse_Alternation_Creates_AlternExpr) {
			parseAndCheck("a|b", "(a|b)");
		}

		TEST_F(RegularExprTest, Parse_Alternation_Is_LeftAssociative) {
			parseAndCheck("a|b|c", "((a|b)|c)");
		}



		TEST_F(RegularExprTest, Parse_Concatenation_Creates_ConcatExpr) {
			parseAndCheck("ab", "(ab)");
		}

		TEST_F(RegularExprTest, Parse_Concatenation_Is_LeftAssociative) {
			parseAndCheck("abc", "((ab)c)");
		}

		TEST_F(RegularExprTest, Parse_Concatenation_Has_HigherPrecedence_Than_Alternation) {
			parseAndCheck("a|bc", "(a|(bc))");
			parseAndCheck("ab|c", "((ab)|c)");
		}



		TEST_F(RegularExprTest, Parse_Optional_Creates_OptionalExpr) {
			parseAndCheck("a?", "a?");
		}

		TEST_F(RegularExprTest, Parse_ZeroOrMore_Creates_StarExpr) {
			parseAndCheck("a*", "a*");
		}

		TEST_F(RegularExprTest, Parse_OneOrMore_Creates_PlusExpr) {
			parseAndCheck("a+", "a+");
		}

		TEST_F(RegularExprTest, Parse_Unary_Has_HigherPrecedence_Than_Binary) {
			parseAndCheck("ab?", "(ab?)");
			parseAndCheck("ab*", "(ab*)");
			parseAndCheck("ab+", "(ab+)");
		}

		TEST_F(RegularExprTest, Parse_Parentheses_Have_HighestPrecedence) {
			parseAndCheck("(a|b)*", "(a|b)*");
		}



		TEST_F(RegularExprTest, Parse_MultiElementCharSet_Creates_AlternExpr) {
			parseAndCheck("[ab]", "(a|b)");
			parseAndCheck("[abc]", "((a|b)|c)");
		}

		TEST_F(RegularExprTest, Parse_SingleElementCharSet_Creates_CharAtom) {
			parseAndCheck("[a]", "a");
		}

		TEST_F(RegularExprTest, Parse_EmptyCharSet_Creates_NilExpr) {
			parseAndCheck("[]", "()");
		}



		TEST_F(RegularExprTest, Parse_MultiElementCharRange_Creates_AlternExpr) {
			parseAndCheck("[a-b]", "(a|b)");
			parseAndCheck("[a-c]", "((a|b)|c)");
		}

		TEST_F(RegularExprTest, Parse_SingleElementCharRange_Creates_CharAtom) {
			parseAndCheck("[a-a]", "a");
		}

		TEST_F(RegularExprTest, Parse_OutOfOrderCharRange_Is_SyntaxError) {
			parseMustThrow("[b-a]");
		}

		TEST_F(RegularExprTest, Parse_BoundaryDash_IsNot_CharRange) {
			parseAndCheck("[a-]", "(a|-)");
			parseAndCheck("[-a]", "(-|a)");
			parseAndCheck("[-]", "-");
		}



		TEST_F(RegularExprTest, Parse_CommonEscapeSeq_Works_AsExpected) {
			parseAndCheck("\\0", "\\0");
			parseAndCheck("\\a", "\\a");
			parseAndCheck("\\b", "\\b");
			parseAndCheck("\\f", "\\f");
			parseAndCheck("\\n", "\\n");
			parseAndCheck("\\r", "\\r");
			parseAndCheck("\\t", "\\t");
			parseAndCheck("\\v", "\\v");
		}

		TEST_F(RegularExprTest, Parse_MetacharEscapeSeq_Produces_Metachar) {
			for(const auto ch : metachars) {
				parseAndCheck(std::format("\\{}", ch), std::format("{}", ch));
			}
		}

		TEST_F(RegularExprTest, Parse_HexEscapeSeq_AllowsOnly_NumbersFrom0To255) {
			parseAndCheck("\\x0", "\\0");
			parseAndCheck("\\xff", "\\xff");
			parseAndCheck("\\xfff", "(\\xfff)");
			
			parseMustThrow("\\xg");
			parseMustThrow("\\");
		}



		TEST_F(RegularExprTest, Parse_IsMetachar_Returns_True_OnlyFor_Metachars) {
			for(int i = 0; i < 256; i++) {
				const auto ch = static_cast<char>(i);
				ASSERT_THAT(metachars,
					Conditional(Parser::isMetachar(ch),
						Contains(ch), Not(Contains(ch))
					)
				);
			}
		}



		TEST_F(RegularExprTest, Pattern_Has_LastChar_As_EndAtom) {
			ASSERT_THAT(Pattern("a|b").endAtom(), AllOf(NotNull(), Property(&CharAtom::value, 'b')));
		}

		TEST_F(RegularExprTest, EmptyPattern_Has_NoEndAtom) {
			ASSERT_THAT(Pattern("").endAtom(), IsNull());
		}

		TEST_F(RegularExprTest, DefaultPattern_Has_NoEndAtom) {
			ASSERT_THAT(Pattern().endAtom(), IsNull());
		}

		TEST_F(RegularExprTest, Pattern_Has_FirstChars_As_RootAtoms) {
			ASSERT_THAT(Pattern("(a|b)c").rootAtoms(), UnorderedElementsAre(
				Property(&CharAtom::value, 'a'),
				Property(&CharAtom::value, 'b')
			));
		}

		TEST_F(RegularExprTest, CharAtom_Has_AtomsImmediatelyToRight_As_Followers) {
			const auto pat = Pattern("a(b|c)");
			ASSERT_THAT(pat.followersOf(pat.rootAtoms().front()), UnorderedElementsAre(
				Property(&CharAtom::value, 'b'),
				Property(&CharAtom::value, 'c')
			));
		}

	}
}