#pragma once
#include "SolidSlice.h"
#include "SolidContour.h"
#include "Mesh_Triangle_List_t.h"
#include "Mesh_DCEL.h"

class IncrementalSlicer
{
public:

	IncrementalSlicer();
	~IncrementalSlicer();

	std::vector <float>* get_planes() { return &this->planes; }
	Mesh_Triangle_List_t** build_buckets(std::vector<float>& planes, std::vector<Triangle>& m, float delta);
	float xround (float x, double eps, int mod, int rem);
	void slice_mesh(Mesh_DCEL& mesh, float layer_thickness, std::vector<SolidSlice> &sliceList);
	void compute_intersection(SolidContour* contour, HalfEdge* edge, int p);
	void define_planes(float thickness, Mesh_DCEL& mesh, float* delta);
	void orient(SolidSlice* slice);
	void define_orientation(int pos, SolidSlice* slice);
	bool create_countours(HalfEdge* itr, SolidContour* countour, int p);
	bool PIP(SolidContour c, Point3D p);

	std::vector<float> planes;
	int intersections;
};

