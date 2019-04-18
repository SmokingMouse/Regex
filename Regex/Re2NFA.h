#pragma once

#include<vector>
#include"RegexTree.h"

extern const bool AcceptS;
extern const bool NonAcceptS;

using State = bool;

//关于边的数据结构我们是可以确定的，包含 StartNode，EndNode， AcceptChar

//关于NFA的数据结构
// 1. NFA保存该NFA中所有的边。保存了所有的边，也就相当于保存了图。但是访问节点的出入边时，效率较低。
// 2. NFA仅保存起点和终点，NFA的边存在于节点自身。
//		2.1 节点保存的是边的另一端的节点，灵活性较差，一旦边被调整，每个节点都要进行调整。
//		2.2 节点保存的是边的指针，指向全局的边池中的边，一旦我们需要调整边，只需调整池便可。

//我们采用2.2的方法，这样数据结构的主体就是边，边的改动间接影响到节点。

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