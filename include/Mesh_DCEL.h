#pragma once

#include <vector>
#include <list>
#include <limits.h>
#include <iostream>
#include "Triangle.h"
#include "HalfEdge.h"
#include "Vertex.h"

class Mesh_DCEL
{
protected:
	std::vector<Vertex> vertices;
	std::vector<halfEdge> edges;
	std::vector<Triangle> faces;
	std::vector<std::vector<int>> face_indices;

	Point3D upperRightVertex;
	Point3D bottomLeftVertex;

public:

	Mesh_DCEL();

	~Mesh_DCEL();

	void roundVertices();

	float xround (float x, double eps, int mod, int rem);

	std::vector<Triangle>& getFaces();
	
	const std::vector<Triangle>& getFaces() const;

	std::vector<std::vector<int>>& getFaceIndices();

	const std::vector<std::vector<int>>& getFaceIndices() const;

	const std::vector<Vertex>& getVertices() const;

	std::vector<Vertex>& getVertices();
	
	std::vector<halfEdge>& getHalfEdges();

	const std::vector<halfEdge>& getHalfEdges() const;

	std::vector<Triangle>& getMesh();

	const std::vector<Triangle>& getMesh() const;

	void setBottomLeftVertex(const Point3D& p);

	void setUpperRightVertex(const Point3D& p);

	Point3D getBottomLeftVertex() const;

	Point3D getUpperRightVertex() const;

	void setUpperBottomVertices(Point3D val);

	Point3D AABBSize();

	void updateTriangles();

	void clear(); 
};