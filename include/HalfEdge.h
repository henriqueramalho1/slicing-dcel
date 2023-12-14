#pragma once
#include "MeshData.h"
#include <iostream>

class Triangle;
class Vertex;
class HalfEdgeNode;

class HalfEdge: public MeshData
{

protected:

	Vertex* origin;
	Triangle* face;
	HalfEdge* next_edge, * previous_edge, * twin_edge;
	HalfEdgeNode* node;

public:

	HalfEdge(int idx, Vertex* start);
	~HalfEdge();

	Triangle* get_face() const;
	HalfEdge* get_next_edge() const;
	HalfEdge* get_previous_edge() const;
	HalfEdge* get_twin_edge() const;
	HalfEdgeNode* get_node();
	Vertex* get_origin() const;
	float get_zmin();
	float get_zmax();
	float get_origin_z();
	float get_destiny_z();
	void set_node(HalfEdgeNode* n);
	void set_face(Triangle* face);
	void set_next_edge(HalfEdge* edge);
	void set_previous_edge(HalfEdge* edge);
	void set_twin_edge(HalfEdge* edge);
	void set_origin(Vertex* o);
	bool is_upward_oriented();
	void print(float z);
};