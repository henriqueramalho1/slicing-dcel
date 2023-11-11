#pragma once
#include <vector>
#include "SolidContour.h"

class SolidSlice
{
public:
	SolidSlice();
	~SolidSlice();

	SolidContour& get_contour(int i);
	void add_contour(SolidContour contour);
	void set_z(float fZ);
	int contour_number();

private:
	
	std::vector<SolidContour> contour_list;	
	float z;
};
