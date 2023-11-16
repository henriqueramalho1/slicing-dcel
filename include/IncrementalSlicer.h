#pragma once
#include "SolidSlice.h"
#include "SolidContour.h"
#include "Mesh_Triangle_List_t.h"
#include "HalfEdgeList.h"
#include "Mesh_DCEL.h"

class IncrementalSlicer
{
public:

	IncrementalSlicer();
	~IncrementalSlicer();

	std::vector <float>* get_planes() { return &this->planes; }
	Mesh_Triangle_List_t** build_buckets(std::vector<float>& planes, std::vector<Triangle>& m, float delta);
	HalfEdgeList** build_edges_buckets(std::vector<float>& planes, std::vector<HalfEdge>& halfedges, float delta);
	SolidContour extract_contour(HalfEdge *h, HalfEdgeList* A, HalfEdgeList* B, float z);
	std::vector<SolidSlice> slice_mesh(Mesh_DCEL& mesh, float layer_thickness);
	Point3D intersection(HalfEdge* h, float z);
	float xround (float x, double eps, int mod, int rem);
	void define_planes(float thickness, Mesh_DCEL& mesh, float* delta);
	ORIENTATION orient(SolidContour contour);

	std::vector<float> planes;
	int intersections;
};

