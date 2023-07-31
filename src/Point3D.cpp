#include "Point3D.h"
//#include "math_functions.h"

Point3D::Point3D(const float x, const float y, const float z): fX(x), fY(y), fZ(z)
{

}

Point3D::Point3D(const Point3D &point): fX(point.get_x()), fY(point.get_y()), fZ(point.get_z())
{
	
}

Point3D::~Point3D(void)
{ 

}

float Point3D::get_x() const
{ return fX; }

float Point3D::get_y() const
{ return fY; }

float Point3D::get_z() const 
{ return fZ; }

void Point3D::set_x (const float x)
{ fX = x; }

void Point3D::set_y (const float y)
{ fY = y; }

void Point3D::set_z (const float z)
{ fZ = z; }

void Point3D::set_point(const Point3D &point)
{
	fX = point.get_x();
	fY = point.get_y();
	fZ = point.get_z();
}

bool Point3D::is_z_bigger(const Point3D &point) const
{
	if( fX > point.get_x())
		return true;
	return false;
}

bool Point3D::is_z_smaller(const Point3D &point) const
{
	return !(is_z_bigger(point));
}

float Point3D::distance (Point3D p)
{
	//Teorema de Pitágoras
	return sqrt( pow(this->fX - p.get_x(),2) + pow(this->fY - p.get_y(),2)
		+ pow(this->fZ - p.get_z(),2));
}

Point3D Point3D::operator+ (const Point3D &point) const
{
	Point3D temp;
	temp.set_x(fX + point.get_x());
	temp.set_y(fY + point.get_y());
	temp.set_z(fZ + point.get_z());
	return temp;
}

Point3D Point3D::operator- (const Point3D &point) const
{
	Point3D temp;
	temp.set_x(fX - point.get_x());
	temp.set_y(fY - point.get_y());
	temp.set_z(fZ - point.get_z());
	return temp;
}

Point3D Point3D::operator- () const
{
	Point3D temp;
	temp.set_x(-fX);
	temp.set_y(-fY);
	temp.set_z(-fZ);
	return temp;
}

Point3D Point3D::operator* (const float &num) const
{
	Point3D temp;
	temp.set_x((double)fX * (double)num);
	temp.set_y((double)fY * (double)num);
	temp.set_z((double)fZ * (double)num);
	return temp;
}

Point3D Point3D::operator/ (const float &num) const
{
	Point3D temp (fX/num, fY/num, fZ/num);
	return temp;
}

void Point3D::operator= (const Point3D &point) 
{
	set_point(point);
}

// bool Point3D::operator== (const Point3D &point) const
// {
// 	if(compf(fX,point.get_x()) && compf(fY,point.get_y())
// 		&& compf(fZ,point.get_z()))
// 		return true;
// 	else return false; 
// 	return true;
// }

// bool Point3D::operator!=(const Point3D &point) const
// {
// 	return !((*this) == point);
// }

bool Point3D::operator< (const Point3D &point) const
{
	return fZ < point.get_z();
}

bool Point3D::operator> (const Point3D &point) const
{
	return fZ > point.get_z();
}

namespace std {

size_t hash<Point3D>::operator()(const Point3D& point) const {
	const size_t x_hash = hash<float>()(point.get_x());
	const size_t y_hash = hash<float>()(point.get_y());
	const size_t z_hash = hash<float>()(point.get_z());
	size_t hash = x_hash;
	hash ^= y_hash + 0x9e3779b97f4a7c15 + (hash << 6) + (hash >> 2);
	hash ^= z_hash + 0x9e3779b97f4a7c15 + (hash << 6) + (hash >> 2);
	return hash;

}
}
