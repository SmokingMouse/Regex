#include"RegexTree.h"
#include"EscapeTable.h"
#include"Util.h"

extern const uint32_t Max;
void getCharFromBitset(const Bitmap& b);

//extern.
RegexTree* getParseResult(const char*);
const char* preProcess(const char*);
RegexTree* parseRegex(const char* , uint32_t);
RegexTree* parseCharClass(const char*, uint32_t&);
RegexTree* parseParallel(const char*, uint32_t,const std::vector<uint32_t>&);
RegexTree* parseCharClass(const char*, uint32_t&);
RegexTree* parseSeries(const char*, uint32_t);

uint32_t findMatchParenPostion(const char*, uint32_t);
bool checkLegality();
int levelDelta(char);
std::vector<uint32_t> parallelPoint(const char*, uint32_t);
bool isRepeatFlag(char);
std::pair<uint32_t, uint32_t> parseTimes(const char*, uint32_t&);
std::pair<uint32_t, uint32_t>getTimes(const char*, uint32_t&);


//defination
RegexTree* getParseResult(const char* regex) {
	const char* new_regex = preProcess(regex);
	uint32_t len = strlen(new_regex);
	RegexTree* result =  parseRegex(new_regex, len);
	delete new_regex;
	return result;
}

//一共有128个字符，char表示256位，我们可以用剩余128位来标识功能。
//我们可以把功能一致性的设置为负数，用以在程序里统一判断。
const char* preProcess(const char* src) {
	uint32_t src_len = strlen(src);
	char* des = new char[src_len+1];
	char* des_ptr = des;
	uint32_t idx = 0;
	bool esc_state = false;
	while (idx < src_len) {
		char cur_char = *(src + idx);
		if (esc_state) {
			*des_ptr++ = -g_escape_table[cur_char] - 1;
			esc_state = false;
		}
		else if (cur_char == '\\') esc_state = true;
		else {
			*des_ptr++ = g_escape_table[cur_char];
		}
		idx++;
	}
	*des_ptr = '\0';
	return des;
}

RegexTree* parseCharClass(const char* regex, uint32_t& idx) {
	CharClass* result = new CharClass();
	if (regex[idx] == g_ctl_left_bracket) {
		++idx;
		bool range = false;
		bool neg = false;
		uint32_t last;
		while (regex[idx] != g_ctl_right_bracket) {
			if (regex[idx] == g_ctl_caret) {
				neg = true;
				++idx;
			}
			else if (regex[idx] == g_ctl_minus) { 
				range = true;
				++idx;
			}
			else if (range) {
 				result->char_class.set(last, regex[idx]);
				range = false;
				++idx;
			}
			else {
				result->char_class.set(regex[idx]);
				last = regex[idx];
				++idx;
			}
		}
		idx++;
		if(neg) result->char_class.reverse();
	}
	else if (regex[idx] == g_ctl_point) {
		result->char_class.all_1();
		idx++;
	}
	else {
		result->char_class.set(regex[idx]);
		idx++;
	}
	return result;
}


RegexTree* parseSeries(const char* regex, uint32_t size) {
	SeriesTree* result = new SeriesTree;
	std::vector<RegexTree*>& trees = result->sub_trees;
	uint32_t idx = 0;
	while (idx < size) {
		char cur_char = regex[idx];
		if (isRepeatFlag(cur_char)) {//1.repeat situation.
			auto times_pair = getTimes(regex, idx);
			if (trees.empty()) throw "unexpected repeat flag occurs.";
			else {
				RegexTree* sub_tree = trees.back();
				trees.pop_back();
				RepeatTree* new_repeat_tree = new RepeatTree;
				new_repeat_tree->min_times = times_pair.first;
				new_repeat_tree->max_times = times_pair.second;
				new_repeat_tree->repeat_tree = sub_tree;
				trees.push_back(new_repeat_tree);
			}
		}
		else if (cur_char == g_ctl_left_paren) {//这里的endParen是相对idx的。
			uint32_t endParen = findMatchParenPostion(regex+idx, size-idx);
			trees.push_back(parseRegex(regex + idx, endParen + 1));
			idx += endParen+1;
		}
		else trees.push_back(parseCharClass(regex, idx));
	}
	return result;
}

uint32_t findMatchParenPostion(const char* regex, uint32_t size) {
	int level = 1;
	uint32_t idx = 0;
	while (level && ++idx < size) {
		if (*(regex+idx) == g_ctl_right_paren) level--;
		else if (*(regex+idx) == g_ctl_left_paren) level++;
	}
	return idx;
}

RegexTree* parseParallel(const char* regex, uint32_t size, const std::vector<uint32_t>& point) {
	ParallelTree* tree = new ParallelTree;

	uint32_t cur = 0;
	for (auto item : point) {
		tree->sub_trees.push_back(parseSeries(regex+cur, item - cur));
		cur = item + 1;
	}
	tree->sub_trees.push_back(parseSeries(regex + cur, size - cur));
	return tree;
}

RegexTree* parseRegex(const char* regex,uint32_t size) {
	if (*regex == g_ctl_left_paren && *(regex + size - 1) == g_ctl_right_paren) 
		return parseRegex(regex + 1, size - 2);
	auto result = parallelPoint(regex, size);
	if (result.empty()) return parseSeries(regex, size);
	else return parseParallel(regex, size, result);
}

//wait to be finished.
bool checkLegality() { return true; }

inline int levelDelta(char c) {
	if (isIn(4,c, g_ctl_left_paren, g_ctl_left_bracket, g_ctl_left_brace)) return 1;
	else if (isIn(4,c, g_ctl_right_paren, g_ctl_right_bracket, g_ctl_right_brace)) return -1;
	else return 0;
}

// dont consider Escape temporarily.
std::vector<uint32_t> parallelPoint(const char* regex, uint32_t size) {
	int level = 0;
	std::vector<uint32_t> result;
	for (uint32_t idx = 0; idx < size; idx++) {
		if (!level && *(regex + idx) == g_ctl_pipe) {
			result.push_back(idx);
			continue;
		}
		level += levelDelta(regex[idx]);
	}
	return result;
}

bool isRepeatFlag(char c) {
	if (isIn(5,c, g_ctl_star, g_ctl_plus, g_ctl_quesmark, g_ctl_left_brace)) return true;
	else return false;
}

std::pair<uint32_t,uint32_t> parseTimes(const char* regex,uint32_t& idx){
	uint32_t fst_time = 0, snd_time = 0;
	bool parseFirst = true;
	while (regex[idx] != g_ctl_right_brace) {
		if (regex[idx] == g_ctl_comma) { parseFirst = false; }
		else if (parseFirst) fst_time += fst_time * 10 + regex[idx] - '0';
		else snd_time += snd_time * 10 + regex[idx] - '0';
		idx++;
	}
	if (!snd_time) snd_time = parseFirst ? fst_time : Max;
	idx++;
	return { fst_time,snd_time };
}

std::pair<uint32_t, uint32_t> getTimes(const char* regex, uint32_t& idx) {
	char cur_char = regex[idx++];
	if (cur_char == g_ctl_star) return { 0,Max };
	if (cur_char == g_ctl_plus) return { 1,Max };
	if (cur_char == g_ctl_quesmark) return { 0,1 };
	if (cur_char == g_ctl_left_brace) return parseTimes(regex, idx);
	throw "error.Expecting repeat char.";
}

