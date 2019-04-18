#pragma once

#include"RegexTree.h"
#include<iostream>
#include"Bitmap.h"

void getCharFromBitset(const Bitmap& b);

int tab = 0;

void writeTab();
void printSeriesTree(SeriesTree*);
void printParallelTree(ParallelTree*);
void printRepeatTree(RepeatTree*);
void printCharClass(CharClass*);
void printRegexTree(RegexTree*);



void writeTab() {
	for (int i = 0; i < tab; i++) {
		std::cout << '\t';
	}
}

void printSeriesTree(SeriesTree* tree) {
	writeTab();
	std::cout << "Series:" << std::endl;
	tab++;
	for (auto item : tree->sub_trees) {
		printRegexTree(item);
	}
	tab--;
}

void printParallelTree(ParallelTree* tree) {
	writeTab();
	std::cout << "Parallel:" << std::endl;
	tab++;
	for (auto item : tree->sub_trees) {
		printRegexTree(item);
	}
	tab--;
}

void printRepeatTree(RepeatTree* tree) {
	writeTab();
	std::cout << "Repeat:" << std::endl;
	tab++;
	printRegexTree(tree->repeat_tree);
	writeTab(); std::cout << "min:" << tree->min_times<<std::endl;
	writeTab(); std::cout << "max:" << tree->max_times << std::endl;
	tab--;
}

void printCharClass(CharClass* tree) {
	writeTab();
	std::cout << "CharClass:" << std::endl;
	tab++;
	writeTab();  
	getCharFromBitset(tree->char_class);
	std::cout << std::endl;
	tab--;
}

void printRegexTree(RegexTree* tree) {
	switch (tree->type)
	{
	case TreeType::series:
		printSeriesTree((SeriesTree*)tree);
		break;
	case TreeType::parallel:
		printParallelTree((ParallelTree*)tree);
		break;
	case TreeType::repeat:
		printRepeatTree((RepeatTree*)tree);
		break;
	case TreeType::charclass:
		printCharClass((CharClass*)tree);
		break;
	default:
		break;
	}
}

void test(const char* regex) {
	auto tree = getParseResult(regex);
	printRegexTree(tree);
}