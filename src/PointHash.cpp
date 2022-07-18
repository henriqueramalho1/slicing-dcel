#include "PointHash.h"
#include <math.h>

PointHash::PointHash():
	hash()
{
}

PointHash::~PointHash()
{
}

void PointHash::initHash(int n)
{
	hash.setHashKeys(n);
}

void PointHash::clearHash()
{
	hash.clear();
}

int PointHash::insert(Point3D p)
{
	int index = getHashIndex(p);

	return hash.insert(p, index);
}

void PointHash::remove(Point3D p)
{
	int index = getHashIndex(p);

	hash.remove(p, index);
}

int PointHash::getHashIndex(Point3D p) const
{
	float x = p.get_x();
	float y = p.get_y();
	float z = p.get_z();

	int vectorModule = static_cast<int>(sqrtf(x * x + y * y + z * z));

	return vectorModule % hash.getKeys();
}

const int PointHash::getKeys() const
{
	return hash.getKeys();
}

void PointHash::initIterator(int index)
{
	hash.initIterator(index);
}

void PointHash::iteratorGoNext()
{
	hash.iteratorGoNext();
}

Point3D PointHash::getIteratorElement()
{
	return hash.getIteratorElement();
}

