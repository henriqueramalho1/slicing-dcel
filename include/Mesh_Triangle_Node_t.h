#pragma once

#include "Triangle.h"

class Mesh_Triangle_Node_t
{
private:
	Triangle* t;
	Mesh_Triangle_Node_t* next;
	Mesh_Triangle_Node_t* prev;
public:
	Mesh_Triangle_Node_t(void);
	~Mesh_Triangle_Node_t(void);
	Triangle* get_t() { return t;}
	Mesh_Triangle_Node_t* get_prev() {return prev;}
	Mesh_Triangle_Node_t* get_next() {return next;}
	void set_t(Triangle* x) {t = x;}
	void set_next(Mesh_Triangle_Node_t* n) {next = n;}
	void set_prev(Mesh_Triangle_Node_t* p) {prev = p;}
};

