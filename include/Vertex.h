#pragma once
#include "Point3D.h"
#include "HalfEdge.h"
#include "MeshData.h"

class Vertex: public MeshData
{
private:

	halfEdge* incidentEdge;
	Point3D point;

public:
	
	Vertex(int idx, Point3D p = Point3D(), halfEdge* ie = nullptr);
	~Vertex();
	void setPoint(Point3D p);
	Point3D& getPoint();
	void setIncidentEdge(halfEdge* edge);
	halfEdge* getIncidentEdge() const;
};

