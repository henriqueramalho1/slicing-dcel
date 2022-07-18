#include "SolidSlice.h"

SolidSlice::SolidSlice()
{
	zCoord = 0.f;
}

SolidSlice::~SolidSlice()
{
	for(int i = 0;i<contourList.size();i++)
	{
		delete(contourList[i]);
	}
	contourList.clear();
}

void SolidSlice::addContour (SolidContour* contour)
{
	contourList.push_back(contour);
}

void SolidSlice::removeContour(const int pos)
{
	if( pos < 0 || pos >= contourList.size())
		return;
	contourList.erase(contourList.begin() + pos);
}

SolidContour* SolidSlice::getContour(int position)
{
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
