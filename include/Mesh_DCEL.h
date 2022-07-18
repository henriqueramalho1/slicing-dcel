#pragma once

#include <vector>
#include <list>
#include <limits.h>
#include <iostream>
#include "Triangle.h"
#include "halfEdge.h"
#include "EdgeIterator.h"
#include "Vertex.h"

#define MESH_NULL_ID UINT_MAX

class Mesh_DCEL
{
protected:
	std::vector<Vertex> vertices;
	std::vector<halfEdge> edges;
	std::vector<Triangle> faces;

	Point3D upperRightVertex;
	Point3D bottomLeftVertex;

	std::list<int> unhandledTriangles;
	unsigned int unhandledTrianglesCount;

public:

	Mesh_DCEL();

	~Mesh_DCEL();

	std::list<int> getUnhandledTriangles() const;

	void manageUnhandledTriangles(); 

	unsigned int createFace(halfEdge* bound, Vertex* v1, Vertex* v2, Vertex* v3);

	unsigned int createTriangularFace(unsigned int vId1, unsigned int vId2, unsigned int vId3);
	
	std::vector<Triangle>& getFaces();
	
	const std::vector<Triangle>& getFaces() const;

	unsigned int getNumFaces() const;

	Triangle* getFace(unsigned int faceId) const;

	unsigned int getFaceId(const Triangle* face) const;

	void checkAllFaces() const;

	void checkFace(unsigned int faceId) const;

	unsigned int createVertex();
	
	Vertex* createGetVertex();

	Vertex* getVertex(unsigned int id) const;

	const std::vector<Vertex>& getVertices() const;

	std::vector<Vertex>& getVertices();

	unsigned int getNumVertices() const;

	unsigned int getVertexId(const Vertex* vertex) const;

	unsigned int createEdge(Vertex* origin, Triangle* face, Vertex* twinOrigin, Triangle* twinFace);
	
	std::vector<halfEdge>& getHalfEdges();

	const std::vector<halfEdge>& getHalfEdges() const;

	halfEdge* getHalfEdge(unsigned int id);

	const halfEdge* getHalfEdge(unsigned int id) const;

	halfEdge* getHalfEdge(Vertex* vertexA, Vertex* vertexB);

	halfEdge* getHalfEdge(unsigned int vertexIdA, unsigned int vertexIdB);

	unsigned int getHalfEdgeId(halfEdge* HalfEdge) const;

	unsigned int getNumHalfEdges() const;
	
	void clear(); 

	halfEdge* findIncidentalHalfEdge(Vertex* vertex);

	std::vector<Triangle>& getMesh();

	const std::vector<Triangle>& getMesh() const;

	void setBottomLeftVertex(const Point3D& p);

	void setUpperRightVertex(const Point3D& p);

	Point3D getBottomLeftVertex() const;

	Point3D getUpperRightVertex() const;

	void setUpperBottomVertices(Point3D val);
};