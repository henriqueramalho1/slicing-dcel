#pragma once
#include "halfEdge.h"
#include "Triangle.h"
#include "Point3D.h"
#include "Vertex.h"

class EdgeIterator
{
protected:
	halfEdge* startEdge;
	halfEdge* nextEdge;
	bool vertexIterator;

public:

	EdgeIterator(const Vertex* vertex);

	EdgeIterator(const Triangle* triangle);

	halfEdge* getNext();

	bool hasNext() const;

	void reset();

	void reset(const Triangle* triangle);

	void reset(const Vertex* vertex);

	void goNextEdge();

	void goTwinEdge();

	void goNext();

	void circleThrough(float z);

	void circleStep(float z);

	halfEdge* get_StartEdge();

	halfEdge* search_Higher(float z);

};

