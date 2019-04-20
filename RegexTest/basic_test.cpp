#include "stdafx.h"
#include "CppUnitTest.h"
#include "..\Regex\Match.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace RegexTest
{		
	TEST_CLASS(Macth_All_Text_Test)
	{
	public:
		
		TEST_METHOD(case1)
		{
			Assert::IsTrue(match_all_text("",""));
			Assert::IsTrue(match_all_text("abc", "abc"));
			Assert::IsTrue(match_all_text("abc", "(abc)"));
		}

		TEST_METHOD(case2) {
			Assert::IsTrue(match_all_text("a", "a|b|c"));
			Assert::IsTrue(match_all_text("b", "a|b|c"));
			Assert::IsTrue(match_all_text("c", "a|b|c"));
			Assert::IsTrue(match_all_text("a", "(a|b|c)"));
			Assert::IsTrue(match_all_text("b", "(a|b|c)"));
			Assert::IsTrue(match_all_text("c", "(a|b|c)"));
		}

		TEST_METHOD(case3) {
			Assert::IsTrue(match_all_text("a", "."));
			Assert::IsTrue(match_all_text("b", "."));
		}

		TEST_METHOD(case4) {
			Assert::IsTrue(match_all_text("", ".*"));
			Assert::IsTrue(match_all_text("abc", ".*"));
			Assert::IsTrue(match_all_text("abc", "(.*)"));
			Assert::IsTrue(match_all_text("abc", "(.*)*"));

			Assert::IsTrue(match_all_text("", "(ab|bc|cd)*"));
			Assert::IsTrue(match_all_text("abbccd", "(ab|bc|cd)*"));

			Assert::IsTrue(match_all_text("b", "(a|b|c)*b"));
			Assert::IsTrue(match_all_text("bb", "(a|b|c)*b"));

			Assert::IsTrue(match_all_text("", "(a|b|c)*b*"));
			Assert::IsTrue(match_all_text("b", "(a|b|c)*b*"));
			Assert::IsTrue(match_all_text("bbb", "(a|b|c)*b*"));
			Assert::IsTrue(match_all_text("bbbabc", "(a|b|c)*b*"));

			Assert::IsTrue(match_all_text("", "(a*)*"));
			Assert::IsTrue(match_all_text("a", "(a*)*"));
			Assert::IsTrue(match_all_text("aa", "(a*)*"));

			Assert::IsTrue(match_all_text("", "a*a*"));
			Assert::IsTrue(match_all_text("a", "a*a*"));
			Assert::IsTrue(match_all_text("aa", "a*a*"));
			Assert::IsTrue(match_all_text("aaa", "a*a*"));

			Assert::IsTrue(match_all_text("", "((ab*|ac*)*|ad*)*"));
			Assert::IsTrue(match_all_text("a", "((ab*|ac*)*|ad*)*"));
			Assert::IsTrue(match_all_text("abbacadaaefa", "((ab*|ac*)*|ad*|ef)*"));
		}

		TEST_METHOD(case5){
			Assert::IsTrue(match_all_text("a", "a+"));
			Assert::IsTrue(match_all_text("a", "(a+)"));
			Assert::IsTrue(match_all_text("a", "(a+)*"));
			Assert::IsTrue(match_all_text("a", "(a+)+"));
			Assert::IsTrue(match_all_text("aa", "(a+)+"));
			Assert::IsTrue(match_all_text("b", "(a+)+|b+"));
			Assert::IsFalse(match_all_text("ba", "(a+)+|b+"));
		}

		TEST_METHOD(case6) {
			Assert::IsTrue(match_all_text("", "a?"));
			Assert::IsTrue(match_all_text("a", "a?"));
			Assert::IsFalse(match_all_text("aa", "a?"));

			Assert::IsTrue(match_all_text("", "a?a?"));
			Assert::IsTrue(match_all_text("", "a?a*"));
			Assert::IsTrue(match_all_text("a", "(a?)"));
			Assert::IsTrue(match_all_text("a", "(a?)*"));
			Assert::IsTrue(match_all_text("a", "(a?)+"));

			Assert::IsTrue(match_all_text("ac", "(ab?c|b)*"));
			Assert::IsTrue(match_all_text("acbbbabcb", "(ab?c|b)*"));

			Assert::IsTrue(match_all_text("aaaaaaaaaaaaaaaaaaaaaaaaaaaaa", "(a?)*a*"));
		}

		TEST_METHOD(case7) {
			Assert::IsTrue(match_all_text("[]", R"(\[\])"));
			Assert::IsTrue(match_all_text("]", R"([\]])"));
			Assert::IsFalse(match_all_text("]", R"([^\]])"));

			Assert::IsTrue(match_all_text("-", R"([\-])"));
			Assert::IsTrue(match_all_text("-", R"([abc\-])"));
			Assert::IsTrue(match_all_text("-", R"([\-abc])"));

			Assert::IsTrue(match_all_text("abc", "[abc]*"));
			Assert::IsTrue(match_all_text("abchij", "[a-ch-ij]*"));

			Assert::IsTrue(match_all_text("xyz", "[^a-ch-ij]*"));
			Assert::IsFalse(match_all_text("acbbbabcb", "[^a-ch-ij]*"));
		}

		TEST_METHOD(case8) {
			Assert::IsTrue(match_all_text("a{", R"(a\{)"));

			Assert::IsTrue(match_all_text("", "a{}"));
			Assert::IsFalse(match_all_text("a", "a{}"));

			Assert::IsTrue(match_all_text("a", R"(a{1})"));
			Assert::IsFalse(match_all_text("aa", R"(a{1})"));

			Assert::IsTrue(match_all_text("a", R"(a{1,})"));
			Assert::IsTrue(match_all_text("aa", R"(a{1,})"));

			Assert::IsTrue(match_all_text("a", "a{1,2}"));
			Assert::IsTrue(match_all_text("aa", "a{1,2}"));
			Assert::IsFalse(match_all_text("aaa", "a{1,2}"));

			Assert::IsTrue(match_all_text("", "a{0,}a*"));
			Assert::IsTrue(match_all_text("", "a{0,}a{0,}"));
			Assert::IsTrue(match_all_text("aa", "(aa?){2}"));
			Assert::IsTrue(match_all_text("aaaaaa", "(a{2}){3}"));
			
			Assert::IsTrue(match_all_text("", "a{0,}b?c*"));
			Assert::IsTrue(match_all_text("a", "a{0,}b?c*"));
			Assert::IsTrue(match_all_text("b", "a{0,}b?c*"));
			Assert::IsTrue(match_all_text("c", "a{0,}b?c*"));
			Assert::IsTrue(match_all_text("ab", "a{0,}b?c*"));
			Assert::IsTrue(match_all_text("ac", "a{0,}b?c*"));
			Assert::IsTrue(match_all_text("bc", "a{0,}b?c*"));
			Assert::IsTrue(match_all_text("abc", "a{0,}b?c*"));

			Assert::IsTrue(match_all_text("xyz", "[^a-ch-ij]*"));
			Assert::IsFalse(match_all_text("acbbbabcb", "[^a-ch-ij]*"));
		}
	};
}