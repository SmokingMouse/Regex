#include"Match.h"
#include"Re2NFA.h"
#include"RegexTree.h"
#include<vector>
#include<unordered_set>
#include<unordered_map>

static uint32_t state_idx;
static std::unordered_map<Node*, uint32_t> last_occur_table;
static std::unordered_map<const char*, NFA*> nfa_cache;

static inline NFA* get_nfa(const char* re) {
	if (nfa_cache.find(re) != nfa_cache.end()) return nfa_cache[re];
	RegexTree* tree = getParseResult(re);
	NFA* new_nfa = Re2NFA(tree);
	nfa_cache[re] = new_nfa;
	delete tree;
	return new_nfa;
}

static inline void exit_work() {
	state_idx = 0;
	last_occur_table.clear();
}

//��ÿ�ζ�һ���ַ�����һ���µ�״̬ʱ����Ҫ������µ�״̬������չ����Ϊ���ܴ��ںܶ�ձߣ�
//��������ӿձ��ն�״̬ʱ��������Ҫ����һ��������������״̬������һ�θ���״̬�г��ֶ�Σ�
//��������ά��һ��map����ʾ�ϴγ�����״̬��ʱ���ǵڼ����ַ�ƥ���ꡣ

//�����ж������״̬Ǩ���У��Ƿ��Ѿ����ֹ���
static inline bool has_added(Node* src) {
	if (last_occur_table.find(src) == last_occur_table.end() ||
		last_occur_table[src] != state_idx) return false;
	return true;
}

static void add_state(Node* src, std::vector<Node*>& state) {
	if (!src || has_added(src)) return;
	last_occur_table[src] = state_idx;
	state.push_back(src);
	for (auto edge : src->outEdges) {
		if (edge->allow.char_class.empty()) {
			add_state(edge->to, state);
		}
	}
	return;
}

//ͨ��ƥ���ַ������������ڵ㵽��ͬһ���ڵ㣬���ǲ������²��裺
//1. �ܷ�ͨ�����ַ�
//2. ͨ���ַ�����ͨ���ձ߷��ʵ����нڵ㡣��add_state)
static void step_to(const std::vector<Node*>& src, char c,std::vector<Node*>& des) {
	for (auto item : src) {
		for (auto edge : item->outEdges) {
			if (edge->allow.char_class.isSet(c)) {
				add_state(edge->to, des);
			}
		}
	}
}

 std::vector<uint32_t> match_points(const char* text, const char* re) {
	 NFA* nfa = get_nfa(re);

	std::vector<uint32_t> result;
	if (!nfa) return result;

	std::vector<Node*> states;
	add_state(nfa->start, states);

	uint32_t idx = 0;
	while (text[idx]) {
		std::vector<Node*> new_states;
		state_idx++;
		step_to(states, text[idx],new_states);
		if (new_states.empty()) {
			exit_work();
			return result;
		}
		for (auto item : new_states) {
			if (item->state == AcceptS) {
				result.push_back(idx);
				break;
			}
		}
		states.assign(new_states.begin(), new_states.end());
		++idx;
	}
	exit_work();
	return result;
}

bool match_all_text(const char* text, const char* re) {
	NFA* nfa = get_nfa(re);

	if (!nfa) return *re == '\0';	
	std::vector<Node*> states;
	add_state(nfa->start, states);

	uint32_t idx = 0;
	while (text[idx]) {
		std::vector<Node*> new_states;
		state_idx++;
		step_to(states, text[idx], new_states);
		if (new_states.empty()) {
			exit_work();
			return false;
		}
		states.assign(new_states.begin(), new_states.end());
		++idx;
	}

	for (auto item : states) {
		if (item->state == AcceptS) {
			exit_work();
			return true;
		}
	}
	exit_work();
	return false;
}