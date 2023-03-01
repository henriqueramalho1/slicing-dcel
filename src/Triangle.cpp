#include "Triangle.h"

Triangle::Triangle(Point3D vertex1, Point3D vertex2,Point3D vertex3)
{
	Point3D* v1 = new Point3D(vertex1);
	Point3D* v2 = new Point3D(vertex2);
	Point3D* v3 = new Point3D(vertex3);
	v[0] = v1;
	v[1] = v2;
	v[2] = v3;

	zMin = std::numeric_limits<float>::max();
	zMax = -std::numeric_limits<float>::max();
	set_z_min_max();
	boundary = NULL;
	state = false;
}

Triangle::Triangle(halfEdge* bound, Vertex* vertex1, Vertex* vertex2, Vertex* vertex3)
{
	
	v[0] = &vertex1->getPoint();
	v[1] = &vertex2->getPoint();
	v[2] = &vertex3->getPoint();
	zMin = std::numeric_limits<float>::max();
	zMax = -std::numeric_limits<float>::max();
	boundary = bound;
	state = false;
}

Triangle::~Triangle(void)
{

}

Point3D Triangle::get_vertex(const int &i) const
{
	return *v[i-1];
}

Point3D Triangle::get_v1() const
{
	return *v[0];
}

Point3D Triangle::get_v2() const
{
	return *v[1];
}

Point3D Triangle::get_v3() const
{
	return *v[2];
}

float Triangle:: get_zmin () const
{
	return zMin;
}

float Triangle:: get_zmax () const
{
	return zMax;
}

void Triangle::set_v1 (const Point3D &point)
{
	v[0]->set_x(point.get_x());
	v[0]->set_y(point.get_y());
	v[0]->set_z(point.get_z());
}

void Triangle::set_v3 (const Point3D &point)
{
	v[2]->set_x(point.get_x());
	v[2]->set_y(point.get_y());
	v[2]->set_z(point.get_z());
}

void Triangle::set_v2 (const Point3D &point)
{
	v[1]->set_x(point.get_x());
	v[1]->set_y(point.get_y());
	v[1]->set_z(point.get_z());
}

void Triangle::set_z_min_max()
{
	for (size_t i = 0; i < 3; ++i)
	{
		if (v[i]->get_z() < zMin)
			zMin = v[i]->get_z();
		if (v[i]->get_z() > zMax)
			zMax = v[i]->get_z();
	}
}

halfEdge* Triangle::get_boundary() const
{
	return boundary;
}

void Triangle::set_boundary(halfEdge* he)
{
	boundary = he;
}
