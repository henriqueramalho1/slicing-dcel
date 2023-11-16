#include "HalfEdge.h"
#include "Triangle.h"
#include "Vertex.h"
#include "HalfEdgeNode.h"

HalfEdge::HalfEdge(int idx, Vertex* start) :
	MeshData(idx)
{
	origin = start;
	face = nullptr;
	node = nullptr;
	next_edge = previous_edge = twin_edge = nullptr;
}

HalfEdge::~HalfEdge()
{

}

Triangle* HalfEdge::get_face() const
{
	return this->face;
}

HalfEdge* HalfEdge::get_next_edge() const
{
	return this->next_edge;
}

HalfEdge* HalfEdge::get_previous_edge() const
{
	return this->previous_edge;
}

HalfEdge* HalfEdge::get_twin_edge() const
{
	return this->twin_edge;
}

HalfEdgeNode *HalfEdge::get_node()
{
    return node;
}

Vertex* HalfEdge::get_origin() const
{
	return this->origin;
}

float HalfEdge::get_zmin()
{
	if(twin_edge->get_origin()->get_point().get_z() < origin->get_point().get_z())
    	return twin_edge->get_origin()->get_point().get_z();

	return origin->get_point().get_z();
}

float HalfEdge::get_zmax()
{
    if(twin_edge->get_origin()->get_point().get_z() < origin->get_point().get_z())
	{
		return twin_edge->get_origin()->get_point().get_z();
	}
    	
	return origin->get_point().get_z();
}

float HalfEdge::get_origin_z()
{
    return origin->get_point().get_z();
}

float HalfEdge::get_destiny_z()
{
    return twin_edge->get_origin()->get_point().get_z();
}

void HalfEdge::set_node(HalfEdgeNode* n)
{
	node = n;
}

void HalfEdge::set_face(Triangle* face)
{
	this->face = face;
}

void HalfEdge::set_next_edge(HalfEdge* edge)
{
	this->next_edge = edge;
	edge->previous_edge = this;
}

void HalfEdge::set_previous_edge(HalfEdge* edge)
{
	previous_edge = edge;
	edge->next_edge = this;
}

void HalfEdge::set_twin_edge(HalfEdge* edge)
{
	this->twin_edge = edge;
	edge->twin_edge = this;
}

void HalfEdge::set_origin(Vertex* o)
{
	origin = o;
}