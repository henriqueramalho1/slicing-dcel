#pragma once

#include "HalfEdgeNode.h"

class HalfEdgeList
{
private:

	HalfEdgeNode* head;
	HalfEdgeNode* tail;
	int list_size;

public:
	HalfEdgeList();
	HalfEdgeList(HalfEdgeNode* h, HalfEdgeNode* t, int size);
	~HalfEdgeList();
	void set_head(HalfEdgeNode* h) {head = h;}
	void set_tail(HalfEdgeNode* t) {tail = t;}
	HalfEdgeNode* get_head() {return head;}
	HalfEdgeNode* get_tail() {return tail;}
	void insert (HalfEdge *h);
	void join (HalfEdgeList *L2);
	HalfEdgeNode* remove (HalfEdgeNode *node);
	void remove_halfedge(HalfEdge* h);
	int size();
    bool is_empty();
};