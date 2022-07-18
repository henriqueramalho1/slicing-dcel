#pragma once
#include "Point3D.h"
#include "HalfEdge.h"

class Vertex
{
private:

	Point3D point;
	halfEdge* incidentEdge;

public:
	
	Vertex(Point3D p = Point3D(), halfEdge* ie = nullptr);
	~Vertex();
	void setPoint(Point3D p);
	Point3D& getPoint();
	void setIncidentEdge(halfEdge* edge);
	halfEdge* getIncidentEdge() const;
};

