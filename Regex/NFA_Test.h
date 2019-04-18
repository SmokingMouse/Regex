#pragma once
#include"Re2NFA.h"
#include"RegexTree.h"
#include<unordered_set>
#include<iostream>
#include<unordered_map>
#include"Bitmap.h"

uint32_t cnt = 0;

extern std::unordered_set<Edge*> g_edge_pool;
extern std::unordered_set<Node*> g_node_pool;

std::unordered_map<Node*, uint32_t> map;

char getCharFromBitset(const Bitmap& b) {
	for (uint8_t i = 0; i < 128; i++) {
		if (b.isSet(i)) return i;
	}
}

void test(const char* re) {
	RegexTree* tree = getParseResult(re);
	NFA* nfa = Re2NFA(tree);
	for (auto item : g_node_pool) {
		map[item] = cnt++;
	}
	for (auto item : g_edge_pool) {
		uint32_t begin_key = map[item->from];
		uint32_t end_key = map[item->to];
		printf("%d\t%d\t%c\t%d\t%d\n",
			item->from->state, begin_key, getCharFromBitset(item->allow.char_class), item->to->state, end_key);
	}
}


