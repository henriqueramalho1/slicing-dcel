#include "Point3D.h"
#include "SolidContour.h"


SolidContour::SolidContour (): PointList(LIST_TYPE::CONTOUR)
{
	
}

SolidContour::~SolidContour()
{
	for(size_t i = 0; i < pointVector.size(); i++){
		delete pointVector[i];
		pointVector[i] = NULL;
	}
	pointVector.clear();
}