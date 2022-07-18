#pragma once

#include <limits>
#include <vector>
#include "Vertex.h"
#include "HalfEdge.h"


class  Triangle
{
public:
	
	Triangle(
		Point3D vertex1 = Point3D(),
		Point3D vertex2 = Point3D(),
		Point3D vertex3 = Point3D());
	Triangle(halfEdge* bound, Vertex* vertex1, Vertex* vertex2, Vertex* vertex3);

	~Triangle(void);

	Point3D get_vertex (const int &i) const;

	Point3D get_v1 () const;
	
	Point3D get_v2 () const;
	
	Point3D get_v3 () const;
	
	Point3D get_incenter();

	float get_zmin () const;
	
	float get_zmax () const;
	
	halfEdge* get_boundary() const;

	void set_v1 (const Point3D &point);

	void set_v2 (const Point3D &point);

	void set_v3 (const Point3D &point);

	void set_boundary(halfEdge* he);

	void set_z_min_max();

	void reset() { state = false; };
	
	void set() { state = true; };

	bool get_state() { return state; };


protected:
	Point3D* v[3];
	halfEdge *boundary;

	float zMin;
	float zMax;
	bool state;

};
