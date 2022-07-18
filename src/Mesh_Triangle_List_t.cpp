#include "Mesh_Triangle_List_t.h"


Mesh_Triangle_List_t::Mesh_Triangle_List_t(void)
{
	head = NULL;
	tail = NULL;
}

Mesh_Triangle_List_t::Mesh_Triangle_List_t(Mesh_Triangle_Node_t* h, Mesh_Triangle_Node_t* t)
{
	head = h;
	tail = t;
}

Mesh_Triangle_List_t::~Mesh_Triangle_List_t(void)
{
}

void Mesh_Triangle_List_t::Mesh_Triangle_List_insert (Triangle *t)
{
	Mesh_Triangle_Node_t *node = (Mesh_Triangle_Node_t *)malloc(sizeof(Mesh_Triangle_Node_t));
	//Triangle* tri = new Triangle(t.get_v1(),t.get_v2(),t.get_v3(),t.get_normal());
	
	node->set_t(t);
	node->set_next(this->get_head());
	node->set_prev(NULL);
	if (this->get_head() == NULL) {
		/*New head*/
		this->set_head(node);
		this->set_tail(node);
	}
	else {
		this->get_head()->set_prev(node);
		this->set_head(node);
	}
}

void Mesh_Triangle_List_t::Mesh_Triangle_List_union (Mesh_Triangle_List_t *L2)
{
	if ( (this->get_head() != NULL) && (L2->get_head() != NULL) ) {
		this->get_tail()->set_next(L2->get_head());
		L2->get_head()->set_prev(this->get_tail());
		this->set_tail(L2->get_tail());
	}
	else if (L2->get_head() != NULL) {
		this->set_head(L2->get_head());
		this->set_tail(L2->get_tail());
	}
}

Mesh_Triangle_Node_t* Mesh_Triangle_List_t::Mesh_Triangle_List_remove (Mesh_Triangle_Node_t *node)
{
	if ((node->get_prev() == NULL) && (node->get_next() == NULL)) {
		free (node);
		this->set_head(NULL);
		this->set_tail(NULL);
		return NULL;
	}
	else if (node->get_prev() == NULL) {
		node->get_next()->set_prev(NULL);
		this->set_head(node->get_next());
		free (node);
		return this->get_head();
	}
	else if (node->get_next() == NULL) {
		node->get_prev()->set_next(NULL);
		this->set_tail(node->get_prev());
		free (node);
		return NULL;
	}
	else {
		Mesh_Triangle_Node_t *next = node->get_next();
		node->get_next()->set_prev(node->get_prev());
		node->get_prev()->set_next(next);
		free (node);
		return next;
  }
}
