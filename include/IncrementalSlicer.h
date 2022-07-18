#pragma once
#include "SolidSlice.h"
#include "SolidContour.h"
#include "Mesh_Triangle_List_t.h"
#include "Mesh_DCEL.h"

class IncrementalSlicer
{
public:

	std::vector<float> planes;

	void sliceMesh(Mesh_DCEL& mesh, float layer_thickness, std::vector<SolidSlice*> &sliceList);

	Mesh_Triangle_List_t** IncrementalSlicing_buildLists(std::vector<float>& planes, std::vector<Triangle>& m, float delta);

	virtual int findOrientation(int pos, SolidSlice* slice);

	void createCountours(halfEdge* itr, SolidContour* countour, int p);

	void computeIntersection(SolidContour* contour, halfEdge* edge, int p);

	void definePlanes(float thickness, Mesh_DCEL& mesh);

	void defineContourOrientation(SolidSlice* slice);

	std::vector <float>* getPlanes() { return &this->planes; }

};

