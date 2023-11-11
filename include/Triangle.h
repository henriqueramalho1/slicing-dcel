#pragma once

#include <limits>
#include <vector>
#include "Vertex.h"
#include "HalfEdge.h"

class  Triangle
{
public:
	
	Triangle(HalfEdge* boundary, Vertex v1, Vertex v2, Vertex v3);
	~Triangle();

	Vertex get_v1 () const;
	Vertex get_v2 () const;
	Vertex get_v3 () const;
	HalfEdge* get_boundary() const;
	float get_zmin () const;
	float get_zmax () const;
	void set_boundary(HalfEdge* b);
	void set_z_min_max();
	void reset() { state = false; };
	void set() { state = true; };
	bool get_state() { return state; };

private:

	std::vector<Vertex> vertices;
	HalfEdge *boundary;
	float z_min;
	float z_max;
	bool state;

};
