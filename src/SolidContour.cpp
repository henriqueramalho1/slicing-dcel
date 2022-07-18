#include "Point3D.h"
#include "SolidContour.h"


SolidContour::SolidContour (): PointList(LIST_TYPE::CONTOUR)
{
	
}

SolidContour::~SolidContour()
{
	for(int i = 0; i < pointVector.size(); i++){
		delete pointVector[i];
		pointVector[i] = NULL;
	}
	pointVector.clear();
}

orientation SolidContour::determinateOrientation()
{
	Point3D* rightest = getPoint(0);
	Point3D* auxNext = nullptr;
	Point3D* auxPrev = nullptr;

	int prevIndex = getCurrentPointIndex();
	int position = 0;

	for(int i = 0; i < getSize();i++)
	{
		if(getPoint(i)->get_x() > rightest->get_x())
		{
			rightest = getPoint(i);
			position =  i;
		}
	}
	
	setPointIndex(position);
	do {
		advancePoint();
		auxNext = getCurrentPoint();
	} while(*auxNext == *rightest);

	setPointIndex(position);
	do {
		returnPoint();
		auxPrev = getCurrentPoint();
	} while(*auxPrev == *rightest);

	setPointIndex(prevIndex);

	//Vetor da distância até o próximo ponto (indicado através de um Point3D)
	Point3D vn(*auxNext-*rightest);
	//Vetor da distância até o ponto anterior (indicado através de um Point3D)
	Point3D vp(*auxPrev-*rightest);

	float xvn = vn.get_x();
	float yvn = vn.get_y();
	float xvp = vp.get_x();
	float yvp = vp.get_y();

	//componente y dos versores (^v = v/|v|)
	float yvnUnit = yvn/sqrt(xvn*xvn + yvn*yvn);
	float yvpUnit = yvp/sqrt(xvp*xvp + yvp*yvp);

	if (yvnUnit > yvpUnit)
		return ORIENTATION::COUNTERCLOCKWISE;
	else if (yvnUnit < yvpUnit)
		return ORIENTATION::CLOCKWISE;
	else return ORIENTATION::UNDEFINED;
}