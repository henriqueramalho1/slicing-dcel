#include "SolidSlice.h"

SolidSlice::SolidSlice()
{
	zCoord = 0.f;
}

SolidSlice::~SolidSlice()
{
	for(size_t i = 0; i<contourList.size();i++)
	{
		delete(contourList[i]);
	}
	contourList.clear();
}

void SolidSlice::addContour (SolidContour* contour)
{
	contourList.push_back(contour);
}

void SolidSlice::removeContour(const int p)
{
	size_t pos = static_cast<size_t>(p);
	if( pos < 0 || pos >= contourList.size())
		return;
	contourList.erase(contourList.begin() + pos);
}

SolidContour* SolidSlice::getContour(int p)
{
	size_t position = static_cast<size_t>(p);
	if(position < contourList.size())
		return contourList[position];
	return NULL;
}

const int SolidSlice::getContourSize ()
{
	return contourList.size();
}

void SolidSlice::setZCoord(float fZ)
{
	zCoord = fZ;
}
