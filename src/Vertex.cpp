#include "Vertex.h"

Vertex::Vertex(int idx, Point3D p):
	MeshData(idx),
	point(p)
{
}

Vertex::~Vertex()
{
	
}

void Vertex::set_point(Point3D p)
{
	point = p;
}

Point3D Vertex::get_point()
{
	return point;
}
