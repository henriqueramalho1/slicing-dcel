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

	int intersections;

	std::vector<float> planes;

	void sliceMesh(Mesh_DCEL& mesh, float layer_thickness, std::vector<SolidSlice*> &sliceList);

	Mesh_Triangle_List_t** IncrementalSlicing_buildLists(std::vector<float>& planes, std::vector<Triangle>& m, float delta);

	bool createCountours(halfEdge* itr, SolidContour* countour, int p);

	void computeIntersection(SolidContour* contour, halfEdge* edge, int p);

	void definePlanes(float thickness, Mesh_DCEL& mesh, float* delta);

	float xround (float x, double eps, int mod, int rem);

	void orient(SolidSlice* slice);

	void defineOrientation(int pos, SolidSlice* slice);

	bool PIP(SolidContour* c, Point3D p);

	std::vector <float>* getPlanes() { return &this->planes; }

};

