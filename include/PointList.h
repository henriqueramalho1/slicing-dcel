#pragma once
#include "Point3D.h"
#include <vector>
#include <algorithm>
#include <iostream>

enum ORIENTATION {
	COUNTERCLOCKWISE,
	CLOCKWISE,
	UNDEFINED,
	CCW = COUNTERCLOCKWISE,
	CW = CLOCKWISE,
	UND = UNDEFINED
};

enum LIST_TYPE { RASTER, OFFSET, CONTOUR };

typedef int listType;
typedef int orientation;

class PointList
{
public:

	PointList(listType _type);

	~PointList();

	void addPoint (Point3D *point);
	
	void insertPoint (Point3D* point, int p);

	Point3D* getPoint (int p);
	
	Point3D* getPointNext (int p);

	Point3D* getPointPrev (int p);

	Point3D* getCurrentPoint();
	
	int getCurrentPointIndex (); 
	
	int getAdvancePointIndex();  
	
	void setPointIndex(int index);
	
	PointList* advancePoint();			  
	
	void returnPoint();	
	
	bool isPointLast();		
	
	bool isPointLast(int index);

	int getSize ();
	
	int getOrientation ();

	void setOrientation (orientation flag);
	
	void removePoint (const int p);

	void reverseList();

	Point3D* getBack();

	Point3D* getFront();

protected:
	
	int currentPoint;
	
	const listType type;
	
	orientation orientationFlag;
	
	std::vector<Point3D*> pointVector;
};