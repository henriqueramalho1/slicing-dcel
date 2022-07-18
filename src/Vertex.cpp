#include "Vertex.h"

Vertex::Vertex(Point3D p, halfEdge* ie):
	incidentEdge(ie),
	point(p)
{
}

Vertex::~Vertex()
{
	incidentEdge = nullptr;
}

void Vertex::setPoint(Point3D p)
{
	point = p;
}

Point3D& Vertex::getPoint()
{
	return point;
}

void Vertex::setIncidentEdge(halfEdge* edge)
{
	incidentEdge = edge;
}

halfEdge* Vertex::getIncidentEdge() const
{
	return incidentEdge;
}
