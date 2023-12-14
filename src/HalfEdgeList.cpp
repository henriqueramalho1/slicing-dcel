#include "HalfEdgeList.h"

HalfEdgeList::HalfEdgeList():
    head(nullptr),
    tail(nullptr),
	list_size(0)
{
}

HalfEdgeList::HalfEdgeList(HalfEdgeNode *h, HalfEdgeNode *t, int size):
    head(h),
    tail(t),
	list_size(size)
{
}

HalfEdgeList::~HalfEdgeList()
{
}

void HalfEdgeList::insert(HalfEdge *h)
{
    HalfEdgeNode *node = (HalfEdgeNode *)malloc(sizeof(HalfEdgeNode));
	
    h->set_node(node);

	node->set_t(h);
	node->set_next(this->get_head());
	node->set_prev(NULL);

	if (this->get_head() == NULL)
    {
		this->set_head(node);
		this->set_tail(node);
	}
	else
    {
		this->get_head()->set_prev(node);
		this->set_head(node);
	}

	list_size++;
}

void HalfEdgeList::join(HalfEdgeList *L2)
{
    if ((this->get_head() != NULL) && (L2->get_head() != NULL))
    {
		this->get_tail()->set_next(L2->get_head());
		L2->get_head()->set_prev(this->get_tail());
		this->set_tail(L2->get_tail());
	}
	else if (L2->get_head() != NULL)
    {
		this->set_head(L2->get_head());
		this->set_tail(L2->get_tail());
	}

	list_size = list_size + L2->size();
}

HalfEdgeNode *HalfEdgeList::remove(HalfEdgeNode *node)
{
    if ((node->get_prev() == NULL) && (node->get_next() == NULL)) {
		free (node);
		this->set_head(NULL);
		this->set_tail(NULL);
		list_size--;
		return NULL;
	}
	else if (node->get_prev() == NULL) {
		node->get_next()->set_prev(NULL);
		this->set_head(node->get_next());
		free (node);
		list_size--;
		return this->get_head();
	}
	else if (node->get_next() == NULL) {
		node->get_prev()->set_next(NULL);
		this->set_tail(node->get_prev());
		free (node);
		list_size--;
		return NULL;
	}
	else {
		HalfEdgeNode *next = node->get_next();
		node->get_next()->set_prev(node->get_prev());
		node->get_prev()->set_next(next);
		free (node);
		list_size--;
		return next;
  	}	
}

void HalfEdgeList::remove_halfedge(HalfEdge *h)
{
	HalfEdgeNode* node = head;

	while(node != NULL)
	{ 
		if (node->get_h() == h)
		{
			remove(node);
			return;
		}
		
		node = node->get_next();
	}
}

int HalfEdgeList::size()
{
	return list_size;
}

bool HalfEdgeList::is_empty()
{
    return (list_size <= 0);
}
