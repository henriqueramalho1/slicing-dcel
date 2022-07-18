#pragma once
#include <iostream>
#include <array>
#include "Hash.h"
#include "Point3D.h"

class PointHash
{
private:

	Hash<Point3D> hash;

public:

	PointHash();
	~PointHash();
	void initHash(int n);
	void clearHash();
	int insert(Point3D p);
	void remove(Point3D p);
	int getHashIndex(Point3D p) const;
	const int getKeys() const;
	void initIterator(int index);
	void iteratorGoNext();
	Point3D getIteratorElement();
};

