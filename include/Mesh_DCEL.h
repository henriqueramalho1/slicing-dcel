#pragma once

#include <vector>
#include <list>
#include <limits>
#include <iostream>
#include "Triangle.h"
#include "HalfEdge.h"
#include "Vertex.h"

class Mesh_DCEL
{
private:
	std::vector<Vertex> vertices;
	std::vector<HalfEdge> edges;
	std::vector<Triangle> faces;
	std::vector<std::vector<int>> face_indices;

	Point3D upper_right_vertex;
	Point3D bottom_left_vertex;

public:

	Mesh_DCEL();
	~Mesh_DCEL();

	float xround (float x, double eps, int mod, int rem);
	std::vector<Triangle>& get_faces();
	std::vector<std::vector<int>>& get_face_indices();
	std::vector<Vertex>& get_vertices();
	std::vector<HalfEdge>& get_halfedges();
	void round_vertices();
	void set_bottom_left_vertex(const Point3D& p);
	void set_upper_right_vertex(const Point3D& p);
	void set_upper_bottom_vertices(Point3D val);
	void update_triangles();
	void clear(); 
	Point3D get_bottom_left_vertex() const;
	Point3D get_upper_right_vertex() const;
	Point3D AABB_size();
};