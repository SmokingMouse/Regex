#include"Re2NFA.h"

#include<unordered_set>

extern const uint32_t Max;
NFA* link_nfa(NFA*, NFA*);
Node* combinate_node(Node*, Node*);


std::unordered_set<Node*> g_node_pool;
std::unordered_set<Edge*> g_edge_pool;

static inline bool is_complex_repeat(RepeatTree* tree) {
	uint32_t min = tree->min_times, max = tree->max_times;
	if ((min <= 1&&max == Max) || (min == 0 && max == 1)) return false;
	else return true;
}

static NFA* complex_repeat_2_NFA(RepeatTree* tree) {
	uint32_t min_times = tree->min_times, max_times = tree->max_times;
	
	Node* start = new Node;
	Node* end = new Node(AcceptS);
	g_node_pool.insert(start);
	g_node_pool.insert(end);

	NFA* pre = (min_times == 0)? NullNFA() : Re2NFA(tree->repeat_tree);
	if (max_times == Max) {
		Edge* edge = new Edge(pre->end, pre->start);
		g_edge_pool.insert(edge);
		pre->end->outEdges.push_back(edge);
	}
	for (uint32_t i = 1; i < min_times; ++i) {
		pre = link_nfa(pre, Re2NFA(tree->repeat_tree));
	}

	Edge* edge_2 = new Edge(pre->end, end);
	g_edge_pool.insert(edge_2);
	pre->end->outEdges.push_back(edge_2);

	for (uint32_t i = 0; i < max_times - min_times&&max_times!=Max; i++) {
		NFA* nfa = Re2NFA(tree->repeat_tree);
		Edge* edge = new Edge(nfa->end, end);
		g_edge_pool.insert(edge);
		nfa->end->outEdges.push_back(edge);
		pre = link_nfa(pre, nfa);
	}
		
	pre->end->state = NonAcceptS;
	Edge* edge_1 = new Edge(start, pre->start);
	g_edge_pool.insert(edge_1);
	start->outEdges.push_back(edge_1);
	delete pre;
	
	return new NFA(start, end);
}

NFA* Re2NFA(RegexTree* tree) {
	switch (tree->type)
	{
	case TreeType::series:return SeriesTree2NFA((SeriesTree*)tree);
	case TreeType::parallel:return ParallelTree2NFA((ParallelTree*)tree);
	case TreeType::repeat:return RepeatTree2NFA((RepeatTree*)tree);
	case TreeType::charclass:return CharClass2NFA((CharClass*)tree);
	default:
		return NULL;
	}
}

NFA* NullNFA() {
	Node* start = new Node;
	Node* end = new Node(AcceptS);
	Edge* edge = new Edge(start, end);

	g_edge_pool.insert(edge);
	g_node_pool.insert(start);
	g_node_pool.insert(end);
	start->outEdges.push_back(edge);
	return new NFA(start, end);
}

//连接两个NFA，变的只可能是NFA状态。
NFA* link_nfa(NFA* l, NFA* r) {
	if (!l && !r) return NULL;
	if (!l) return r;
	if (!r) return l;
	combinate_node(l->end, r->start);
	l->end = r->end;
	delete r;
	return l;
}


//融合两个节点，在融合过程中，边没有消失，只是边的起始和结束转移了。
//对于融合操作，只有两个变化。
// 1. 边的变化。
// 2. 节点内容的变化，于节点本身而言，变的只可能是出边集和状态。
Node* combinate_node(Node* l, Node* r) {
	//l和r都在池中
	if (l == r) return l;
	for (auto item : r->outEdges) item->from = l;
	l->outEdges.insert(l->outEdges.end(), r->outEdges.begin(), r->outEdges.end());
	l->state = r->state;
	g_node_pool.erase(r);
	delete r;
	return l;
}

NFA* SeriesTree2NFA(SeriesTree* tree) {
	const auto& subtrees = tree->sub_trees;
	NFA* result = NULL;
	for (auto item : subtrees) {
		NFA* temp_nfa = Re2NFA(item);
		result = link_nfa(result, temp_nfa);
	}
	return result;
}

//对于每个并列的NFA进行如下几步操作
//1. 在pool里添加两条新边
//2. 为new_start节点和old_end节点添加新边。
//3. 更改old_end节点的状态
//4. 释放掉旧的NFA

NFA* ParallelTree2NFA(ParallelTree* tree) {
	Node* new_start = new Node(NonAcceptS);
	Node* new_end = new Node(AcceptS);
	const auto& subtrees = tree->sub_trees;
	g_node_pool.insert(new_end);
	g_node_pool.insert(new_start);
	NFA* result = new NFA(new_start,new_end);
	for (auto item : subtrees) {
		NFA* temp = Re2NFA(item);
		Edge* edge_1 = new Edge(new_start, temp->start);
		g_edge_pool.insert(edge_1);
		Edge* edge_2 = new Edge(temp->end, new_end);
		g_edge_pool.insert(edge_2);//1
		new_start->outEdges.push_back(edge_1);
		temp->end->outEdges.push_back(edge_2);//2
		temp->end->state = NonAcceptS;//3
		delete temp;//4
	}
	return result;
}



NFA* RepeatTree2NFA(RepeatTree* tree) {
	if (is_complex_repeat(tree)) return complex_repeat_2_NFA(tree);

	NFA* old_nfa = Re2NFA(tree->repeat_tree);
	Node* new_start = new Node(NonAcceptS);
	Node* new_end = new Node(AcceptS);
	Node* old_start = old_nfa->start;
	Node* old_end = old_nfa->end;
	old_end->state = NonAcceptS;
	g_node_pool.insert(new_start);
	g_node_pool.insert(new_end);

	NFA* result = new NFA(new_start, new_end);

	uint32_t min_time = tree->min_times;
	uint32_t max_time = tree->max_times;

	Edge* zero_edge = NULL;
	Edge* infinite_edge = NULL;
	Edge* edge_1 = new Edge(new_start, old_start);
	Edge* edge_2 = new Edge(old_end, new_end);
	g_edge_pool.insert(edge_1);
	g_edge_pool.insert(edge_2);
	if (!min_time) {
		zero_edge = new Edge(new_start, new_end);
		g_edge_pool.insert(zero_edge);
		new_start->outEdges.push_back(zero_edge);
	}
	if (max_time == Max) {
		infinite_edge = new Edge(old_end, old_start);
		g_edge_pool.insert(infinite_edge);
		old_end->outEdges.push_back(infinite_edge);
	}
	new_start->outEdges.push_back(edge_1);
	old_end->outEdges.push_back(edge_2);
	delete old_nfa;
	return result;
}

NFA* CharClass2NFA(CharClass* tree) {
	Node* new_start = new Node(NonAcceptS);
	Node* new_end = new Node(AcceptS);
	Edge* new_edge = new Edge(new_start, new_end, *tree);

	new_start->outEdges.push_back(new_edge);

	g_edge_pool.insert(new_edge);
	g_node_pool.insert(new_start);
	g_node_pool.insert(new_end);

	NFA* result = new NFA(new_start, new_end);
	return result;
}