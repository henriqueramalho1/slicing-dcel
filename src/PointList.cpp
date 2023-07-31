#include "PointList.h"

PointList::PointList(listType _type) : type(_type), pointVector()
{
	orientationFlag = -1;
	currentPoint = 0;
	pointVector.clear();
}

PointList::~PointList()
{
	for (size_t i = 0; i < pointVector.size(); i++) {
		delete pointVector[i];
	}
	pointVector.clear();
}

void PointList::addPoint(Point3D* point)
{

	pointVector.push_back(point);
}

void PointList::insertPoint(Point3D* point, int p)
{
	size_t position = static_cast<size_t>(p);
	if (position >= 0 && position <= pointVector.size())
	{
		auto it = pointVector.begin() + position;
		pointVector.insert(it, point);
	}
	else throw ("Index out of range!");
}

Point3D* PointList::getPoint(int p)
{
	size_t position = static_cast<size_t>(p);

	if (position >= 0 && position < pointVector.size())
	{
		return pointVector[position];
	}
	if (position < 0) {
		while (position < 0) {
			position += pointVector.size();
		}
		return pointVector[position];
	}
	if (position >= pointVector.size() && pointVector.size() != 0) {
		while (position >= pointVector.size()) {
			position -= pointVector.size();
		}
		return pointVector[position];
	}
	else throw("Index out of range!");
}

Point3D* PointList::getPointNext(int p)
{
	size_t position = static_cast<size_t>(p);

	if (position + 1 < pointVector.size())
		return pointVector[position + 1];
	return pointVector[0];
}

Point3D* PointList::getPointPrev(int p)
{
	size_t position = static_cast<size_t>(p);

	if (position == 0)
		return pointVector[pointVector.size() - 1];
	return pointVector[position - 1];
}

int PointList::getCurrentPointIndex()
{
	return currentPoint;
}

int PointList::getAdvancePointIndex()
{
	advancePoint();
	return currentPoint;
}

void PointList::setPointIndex(int i)
{
	size_t index = static_cast<size_t>(i);

	if (index >= 0 && index < pointVector.size())
		currentPoint = index;
	else  throw("Index out of range!");
}

PointList* PointList::advancePoint()
{
	currentPoint++;
	if (currentPoint >= int(pointVector.size()))
		currentPoint = 0;

	return this;
}

void PointList::returnPoint()
{
	currentPoint--;
	if (currentPoint < 0)
		currentPoint = pointVector.size() - 1;
}

bool PointList::isPointLast()
{
	return currentPoint == int(pointVector.size()) - 1;
}

bool PointList::isPointLast(int index)
{
	return index == int(pointVector.size()) - 1;
}

int PointList::getSize()
{
	return pointVector.size();
}

int PointList::getOrientation()
{
	return orientationFlag;
}

void PointList::setOrientation(orientation flag)
{
	switch (flag)
	{
	case ORIENTATION::COUNTERCLOCKWISE:
		orientationFlag = ORIENTATION::COUNTERCLOCKWISE;
		break;
	case ORIENTATION::CLOCKWISE:
		orientationFlag = ORIENTATION::CLOCKWISE;
		break;
	default:
		orientationFlag = ORIENTATION::UNDEFINED;
	}
}

void PointList::removePoint(const int p)
{
	size_t pos= static_cast<size_t>(p);
	if (pos < 0 || pos >= pointVector.size())
		return;

	pointVector.erase(pointVector.begin() + pos);

}

void PointList::reverseList()
{
	std::reverse(pointVector.begin(), pointVector.end());
}

Point3D* PointList::getCurrentPoint()
{
	return pointVector[currentPoint];
}

Point3D* PointList::getBack()
{
	return pointVector.back();
}

Point3D* PointList::getFront()
{
	return pointVector.front();
}
