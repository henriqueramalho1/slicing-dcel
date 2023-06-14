#include "HalfEdge.h"
#include "Triangle.h"
#include "Vertex.h"

halfEdge::halfEdge(int idx, Vertex* start) :
	MeshData(idx)
{

	origin = start;
	face = nullptr;
	nextEdge = previousEdge = twin = nullptr;
}

halfEdge::halfEdge():
	MeshData(-1)
{
	origin = NULL;
	face = nullptr;
	nextEdge = previousEdge = twin = nullptr;
}

halfEdge::~halfEdge() {};

Vertex* halfEdge::get_startVertex() const
{
	return this->origin;
}


Triangle* halfEdge::get_Face() const
{
	return this->face;
}

halfEdge* halfEdge::get_nextEdge() const
{
	return this->nextEdge;
}

halfEdge* halfEdge::get_previousEdge() const
{
	return this->previousEdge;
}

halfEdge* halfEdge::get_twinEdge() const
{
	return this->twin;
}

Vertex* halfEdge::getOrigin() const
{
	return this->origin;
}

void halfEdge::set_startVertex(Vertex* point)
{
	origin = point;
}


void halfEdge::set_Face(Triangle* face)
{
	this->face = face;
}

void halfEdge::set_nextEdge(halfEdge* edge)
{
	this->nextEdge = edge;
	edge->previousEdge = this;
}

void halfEdge::set_previousEdge(halfEdge* edge)
{
	previousEdge = edge;
	edge->nextEdge = this;
}

void halfEdge::set_twinEdge(halfEdge* edge)
{
	this->twin = edge;
	edge->twin = this;
}

void halfEdge::setOrigin(Vertex* o)
{
	origin = o;
}