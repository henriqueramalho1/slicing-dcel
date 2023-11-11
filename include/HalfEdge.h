#pragma once
#include "MeshData.h"
#include <iostream>

class Triangle;
class Vertex;

class HalfEdge: public MeshData
{

protected:

	Vertex* origin;
	Triangle* face;
	HalfEdge* next_edge, * previous_edge, * twin_edge;

public:

	HalfEdge(int idx, Vertex* start);
	~HalfEdge();

	Triangle* get_face() const;
	HalfEdge* get_next_edge() const;
	HalfEdge* get_previous_edge() const;
	HalfEdge* get_twin_edge() const;
	Vertex* get_origin() const;
	void set_face(Triangle* face);
	void set_next_edge(HalfEdge* edge);
	void set_previous_edge(HalfEdge* edge);
	void set_twin_edge(HalfEdge* edge);
	void set_origin(Vertex* o);
};