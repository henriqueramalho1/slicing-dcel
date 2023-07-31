#pragma once
#include "PointList.h"
#include <math.h>

class Point3D;

class SolidContour : public PointList
{
public:

	SolidContour();

	~SolidContour();

	std::vector<Point3D*> getPointVector(){return pointVector;}
};
