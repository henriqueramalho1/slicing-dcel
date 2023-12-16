#include "Triangle.h"

Triangle::Triangle(HalfEdge* boundary, Vertex v1, Vertex v2, Vertex v3):
	boundary(boundary),
	z_min(std::numeric_limits<float>::max()),
	z_max(-std::numeric_limits<float>::max())
{
	vertices.push_back(v1);
	vertices.push_back(v2);
	vertices.push_back(v3);
	set_z_min_max();
}

Triangle::~Triangle(void)
{

}

Vertex Triangle::get_v1() const
{
	return vertices[0];
}

Vertex Triangle::get_v2() const
{
	return vertices[1];
}

Vertex Triangle::get_v3() const
{
	return vertices[2];
}

float Triangle:: get_zmin () const
{
	return z_min;
}

float Triangle:: get_zmax () const
{
	return z_max;
}

void Triangle::set_boundary(HalfEdge *b)
{
	boundary = b;
}

void Triangle::set_z_min_max()
{
	z_min = std::numeric_limits<float>::max();
	z_max = -std::numeric_limits<float>::max();

	for (int i = 0; i < 3; ++i)
	{
		if (vertices[i].get_point().get_z() < z_min)
			z_min = vertices[i].get_point().get_z();

		if (vertices[i].get_point().get_z() > z_max)
			z_max = vertices[i].get_point().get_z();
	}
}

HalfEdge* Triangle::get_boundary() const
{
	return boundary;
}
