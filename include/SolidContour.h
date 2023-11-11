#pragma once
#include "Point3D.h"

#include <vector>
#include <algorithm>

class Point3D;

enum ORIENTATION {
	CCW,
	CW
};

class SolidContour
{
public:

	SolidContour();
	~SolidContour();

	std::vector<Point3D> get_points();
	Point3D& get_point(int i);
	int get_size();
	void add_point(Point3D p);
	void set_orientation(ORIENTATION o);
	void reverse_point_list();
	ORIENTATION get_orientation();

private:

	std::vector<Point3D> point_list;
	ORIENTATION orientation;
};
