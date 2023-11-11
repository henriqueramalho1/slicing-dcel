#include "Point3D.h"

Point3D::Point3D(const float x, const float y, const float z): fX(x), fY(y), fZ(z)
{

}

Point3D::Point3D(const Point3D &point): 
	fX(point.get_x()),
	fY(point.get_y()),
	fZ(point.get_z())
{
	
}

Point3D::~Point3D(void)
{ 

}

float Point3D::get_x() const
{ 
	return fX;
}

float Point3D::get_y() const
{ 
	return fY;
}

float Point3D::get_z() const 
{ 
	return fZ;
}

void Point3D::set_x (const float x)
{ 
	fX = x;
}

void Point3D::set_y (const float y)
{ 
	fY = y;
}

void Point3D::set_z (const float z)
{	
	fZ = z;
}

void Point3D::set_point(const Point3D &point)
{
	fX = point.get_x();
	fY = point.get_y();
	fZ = point.get_z();
}