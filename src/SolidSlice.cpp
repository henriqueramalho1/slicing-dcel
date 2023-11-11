#include "SolidSlice.h"

SolidSlice::SolidSlice():
	z(0.f)
{

}

SolidSlice::~SolidSlice()
{
	contour_list.clear();
	contour_list.shrink_to_fit();
}

void SolidSlice::add_contour(SolidContour contour)
{
	contour_list.push_back(contour);
}

SolidContour& SolidSlice::get_contour(int i)
{
	return contour_list[i];
}

int SolidSlice::contour_number()
{
	return contour_list.size();
}

void SolidSlice::set_z(float fZ)
{
	z = fZ;
}
