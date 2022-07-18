#pragma once
#include <vector>
#include "Point3D.h"
#include "SolidContour.h"

class SolidSlice
{
public:
	SolidSlice();

	~SolidSlice();

	void addContour(SolidContour* contour);

	void removeContour(const int pos);

	SolidContour* getContour(int position);

	const int getContourSize();

	void setZCoord(float fZ);


private:
	
	std::vector<SolidContour*> contourList;	
	float zCoord;
};
