#include "SolidContour.h"

SolidContour::SolidContour():
	point_list(),
	orientation(ORIENTATION::CCW)
{
	
}

SolidContour::~SolidContour()
{
	point_list.clear();
	point_list.shrink_to_fit();
}

std::vector<Point3D> SolidContour::get_points()
{
    return point_list;
}

Point3D& SolidContour::get_point(int i)
{
    return point_list[i];
}

int SolidContour::get_size()
{
    return point_list.size();
}

void SolidContour::add_point(Point3D p)
{
	point_list.push_back(p);
}

void SolidContour::set_orientation(ORIENTATION o)
{
	orientation = o;
}

void SolidContour::reverse_point_list()
{
	std::reverse(point_list.begin(), point_list.end());
}

ORIENTATION SolidContour::get_orientation()
{
    return orientation;
}
