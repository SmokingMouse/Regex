#pragma once

#include<vector>
#include"RegexTree.h"

extern const bool AcceptS;
extern const bool NonAcceptS;

using State = bool;

//���ڱߵ����ݽṹ�����ǿ���ȷ���ģ����� StartNode��EndNode�� AcceptChar

//����NFA�����ݽṹ
// 1. NFA�����NFA�����еıߡ����������еıߣ�Ҳ���൱�ڱ�����ͼ�����Ƿ��ʽڵ�ĳ����ʱ��Ч�ʽϵ͡�
// 2. NFA�����������յ㣬NFA�ıߴ����ڽڵ�����
//		2.1 �ڵ㱣����Ǳߵ���һ�˵Ľڵ㣬����Խϲһ���߱�������ÿ���ڵ㶼Ҫ���е�����
//		2.2 �ڵ㱣����Ǳߵ�ָ�룬ָ��ȫ�ֵı߳��еıߣ�һ��������Ҫ�����ߣ�ֻ������ر�ɡ�

//���ǲ���2.2�ķ������������ݽṹ��������Ǳߣ��ߵĸĶ����Ӱ�쵽�ڵ㡣

class Edge;
class NFA;
class Node;

class NFA {
public:
	NFA() = default;
	NFA(Node* s,Node* e):start(s),end(e){}
	Node* start;
	Node* end;
};

class Edge {
public:
	Edge() = default;
	Edge(Node* f,Node* t):from(f),to(t){}
	Edge(Node* f,Node* t,CharClass c):from(f),to(t),allow(c){}
	CharClass allow;
	Node* from;
	Node* to;
};

class Node {
public:
	Node():state(NonAcceptS){}
	Node(State s):state(s){}
	std::vector<Edge*> outEdges;
	State state;
};

NFA* Re2NFA(RegexTree*);
NFA* SeriesTree2NFA(SeriesTree*);
NFA* ParallelTree2NFA(ParallelTree*);
NFA* RepeatTree2NFA(RepeatTree*);
NFA* CharClass2NFA(CharClass*);
NFA* NullNFA();