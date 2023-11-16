#pragma once

#include "HalfEdge.h"

class HalfEdgeNode
{
private:

	HalfEdge* h;
	HalfEdgeNode* next;
	HalfEdgeNode* prev;

public:
	HalfEdgeNode();
	~HalfEdgeNode();
	HalfEdge* get_h() { return h;}
	HalfEdgeNode* get_prev() {return prev;}
	HalfEdgeNode* get_next() {return next;}
	void set_t(HalfEdge* x) {h = x;}
	void set_next(HalfEdgeNode* n) {next = n;}
	void set_prev(HalfEdgeNode* p) {prev = p;}
};

