#pragma once
#include "Point3D.h"
#include "HalfEdge.h"
#include "MeshData.h"

class Vertex: public MeshData
{
private:

	Point3D point;

public:
	
	Vertex(int idx, Point3D p);
	~Vertex();
	Point3D get_point();
};

