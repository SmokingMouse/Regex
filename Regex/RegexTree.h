#pragma once

#include<string>
#include<vector>
//#include<bitset>     本来准备使用这个，不支持-操作，没办法。
#include"Bitmap.h"

enum class TreeType{series,parallel,repeat,charclass};


class RegexTree {
public:
	TreeType type;
	virtual ~RegexTree(){}
};

class SeriesTree : public RegexTree {
public:
	SeriesTree() { type = TreeType::series; }
	std::vector<RegexTree*> sub_trees;
};

class ParallelTree : public RegexTree {
public:
	ParallelTree() { type = TreeType::parallel; }
	std::vector<RegexTree*> sub_trees;
};

class RepeatTree : public RegexTree{
public:
	RepeatTree() { type = TreeType::repeat; }
	RegexTree* repeat_tree;
	uint32_t min_times;
	uint32_t max_times;
};

struct CharClass : public RegexTree {
public:
	CharClass() { type = TreeType::charclass; }
	Bitmap char_class;
};


RegexTree* getParseResult(const char* regex);