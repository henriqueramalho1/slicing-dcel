#include "Vertex.h"

Vertex::Vertex(int idx, Point3D p):
	MeshData(idx),
	point(p)
{
}

Vertex::~Vertex()
{
	
}

Point3D Vertex::get_point()
{
	return point;
}
