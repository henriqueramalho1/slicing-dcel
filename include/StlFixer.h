#pragma once

#include <iostream>
#include <vector>
#include <array>
#include "Point3D.h"
#include "PointHash.h"

class StlFixer
{
private:

	static StlFixer* instance;
	PointHash hash;
	StlFixer();
	~StlFixer();

public:

	static StlFixer* getInstance();
	/**
		Round float coordinates to 6 decimals
	*/
	void roundPoint(float* v1, float* v2, float* v3);
	void checkDuplicates(std::vector<std::array<Point3D, 3>> *triangles);
	float calcDistance(Point3D p1, Point3D p2);
	int serchPrime(int n);
	void showHash();
	void initHash(int n);
	void clearHash();
};

