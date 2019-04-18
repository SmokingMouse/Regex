#include"RegexTree.h"
#include"NFA_Test.h"
//#include"RegexTreeTest.h"
#include"Re2NFA.h"
#include<stdio.h>
#include<string>
#include"Util.h"
#include"Match.h"

int main() {
	auto result = match_all_text("helloworld", R"(hel{2,}oworld)");
//	test(R"(hel{2,})");
//	test(R"(l{1,1})");
}