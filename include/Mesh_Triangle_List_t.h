#pragma once

#include "Mesh_Triangle_Node_t.h"

class Mesh_Triangle_List_t
{
private:
	Mesh_Triangle_Node_t* head;
	Mesh_Triangle_Node_t* tail;
public:
	Mesh_Triangle_List_t(void);
	Mesh_Triangle_List_t(Mesh_Triangle_Node_t* h,Mesh_Triangle_Node_t* t);
	~Mesh_Triangle_List_t(void);
	void set_head(Mesh_Triangle_Node_t* h) {head = h;}
	void set_tail(Mesh_Triangle_Node_t* t) {tail = t;}
	Mesh_Triangle_Node_t* get_head() {return head;}
	Mesh_Triangle_Node_t* get_tail() {return tail;}
	void Mesh_Triangle_List_insert (Triangle *t);
	void Mesh_Triangle_List_union ( Mesh_Triangle_List_t *L2);
	Mesh_Triangle_Node_t* Mesh_Triangle_List_remove (Mesh_Triangle_Node_t *node);
};

