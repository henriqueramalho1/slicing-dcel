#include "IncrementalSlicer.h"
#include "Mesh_DCEL.h"
#include <iostream>
#include <fstream>
#include <math.h>
#include <chrono>
#include "HalfEdge.h"
#include "math_functions.h"

IncrementalSlicer::IncrementalSlicer():
	intersections(0)
{

}

IncrementalSlicer::~IncrementalSlicer()
{
	
}

void IncrementalSlicer::sliceMesh(Mesh_DCEL& mesh, float layer_thickness, std::vector<SolidSlice*> &sliceList)
{
	float delta = 0.0f;

	definePlanes(layer_thickness, mesh, &delta);

	std::vector<Triangle>& m = mesh.getFaces();

	size_t nplanes = planes.size();

	Mesh_Triangle_List_t** L = IncrementalSlicing_buildLists(planes, m, delta);

	Mesh_Triangle_List_t* A = new Mesh_Triangle_List_t;

	for (int p = 0; p < planes.size(); p++) {
		/* Add triangles that start between {P[p-1]} and {P[p]}: */
		A->Mesh_Triangle_List_union(L[p]);
		/* Scan the active triangles: */
		Mesh_Triangle_Node_t* aux = A->get_head();

		while (aux != NULL) {
			Mesh_Triangle_Node_t* next = aux->get_next();
			if (aux->get_t()->get_zmax() < planes[p]) {
				/* Triangle is exhausted: */
				A->Mesh_Triangle_List_remove(aux);
			}
			aux = next;

		}

		if (A->get_head() == NULL)
		{
			//std::cout << p <<" \tRemoved all, going to next plane" << std::endl;
			continue;
		}

		halfEdge* itr = A->get_head()->get_t()->get_boundary();

		SolidSlice* slice = new SolidSlice();// new slice for solid

		aux = A->get_head();
		do
		{
			Mesh_Triangle_Node_t* next = aux->get_next();

			if (aux->get_t()->get_state() == false) {

				Point3D v1 = aux->get_t()->get_v1();
				Point3D v2 = aux->get_t()->get_v2();
				Point3D v3 = aux->get_t()->get_v3();

				SolidContour* contour = new SolidContour();

				itr = aux->get_t()->get_boundary();

				if(createCountours(itr, contour, p))
					slice->addContour(contour);//adiciona contorno na slice
			}
			aux = next;
		} while (aux != NULL);

		slice->setZCoord(planes[p]);

		defineContourOrientation(slice); //define a orientação dos contornos
		sliceList.push_back(slice);
		//std::cout << "Slice [" << sliceList.size() - 1 << "] computed --> Size: " << sliceList[sliceList.size() - 1]->getContourSize() << std::endl;

		aux = A->get_head();
		while (aux != NULL) {
			Mesh_Triangle_Node_t* next = aux->get_next();
			aux->get_t()->reset();
			aux = next;
		}

	}
	free(L);
}


Mesh_Triangle_List_t** IncrementalSlicer::IncrementalSlicing_buildLists(std::vector<float>& planes, std::vector<Triangle>& mesh, float delta)
{
	int k = planes.size();
	Mesh_Triangle_List_t** L = (Mesh_Triangle_List_t**)malloc((k + 1) * sizeof(Mesh_Triangle_List_t*));

	for (size_t p = 0; p <= k; p++) {
		L[p] = new Mesh_Triangle_List_t;
	}

	int n = mesh.size(); /* Number of triangles. */

	if (delta > 0.0) {
		for (int i = 0; i < n; ++i) {
			Triangle* t = &mesh[i];
			int p;
			if (t->get_zmin() < planes[0]) {
				p = 0;
			}
			else if (t->get_zmin() > planes[k-1]) {
				p = k;
			}
			else {
				p = floor((t->get_zmin() - planes[0])/delta) + 1;
			}
			L[p]->Mesh_Triangle_List_insert(t);
		}
	}
	else {
		for (int i = 0; i < n; ++i) {
			Triangle* t = &mesh[i];
			t->set_z_min_max();
			//Binary search
			/* Assumes that {planes} is a list of {k} strictly increasing {Z} coordinates.
			Returns an integer {p} such that {planes[p-1] < zMin < planes[p]}. As special cases,
			if {zMin < planes[0]} returns 0; if {zMin > planes[k-1]} returns {k}. */
			int p;
			if (t->get_zmin() >= planes[k - 1]) { p = k; }
			int l = -1; /* Inferior Z index. */
			int r = k;  /* Superior Z index. */
			while (r - l > 1) {
				/* At this point, {zMin} is between {P[l]} and {P[r]}. */
				int m = (l + r) / 2;
				if (t->get_zmin() >= planes[m]) {
					l = m;
				}
				else {
					r = m;
				}
			}
			p = r;
			// end Binary search
			L[p]->Mesh_Triangle_List_insert(t);
		}
	}
	return L;
}

bool IncrementalSlicer::createCountours(halfEdge* itr, SolidContour* contour, int p)
{
	halfEdge* const start = itr;
	halfEdge* next;
	halfEdge* prev;

	do
	{
		next = itr->get_nextEdge();
		prev = itr->get_previousEdge();

		if(next == nullptr || prev == nullptr)
		{
			delete contour;
			return false;
		}

		if ((next->getOrigin()->getPoint().get_z() < planes[p] && next->get_twinEdge()->getOrigin()->getPoint().get_z() > planes[p])
			||
			(next->getOrigin()->getPoint().get_z() > planes[p] && next->get_twinEdge()->getOrigin()->getPoint().get_z() < planes[p]))
		{
			computeIntersection(contour, next, p);

			if (itr->get_Face() != NULL)
			{
				itr->get_Face()->set();
			}

			itr = next;
		}
		else if ((prev->getOrigin()->getPoint().get_z() < planes[p] && prev->get_twinEdge()->getOrigin()->getPoint().get_z() > planes[p])
			||
			(prev->getOrigin()->getPoint().get_z() > planes[p] && prev->get_twinEdge()->getOrigin()->getPoint().get_z() < planes[p]))
		{
			computeIntersection(contour, prev, p);

			if (itr->get_Face() != NULL)
			{
				itr->get_Face()->set();
			}

			itr = prev;

		}
		else
		{
			delete contour;
			return false;
		}

		itr = itr->get_twinEdge();
		intersections++;

		bool ending = false;
		if(itr == start || itr->get_nextEdge() == start || itr->get_twinEdge() == start || itr->get_previousEdge() == start)
		{
			itr = start;
			ending = true;
		}

		if(ending)
		{
			Point3D* initPoint = new Point3D(*contour->getPointVector()[0]);
			contour->addPoint(initPoint);
		}

	} while (itr != start);

	return true;
}

void IncrementalSlicer::computeIntersection(SolidContour* contour, halfEdge* edge, int p)
{
	float topVertice[3] = { edge->getOrigin()->getPoint().get_x() , edge->getOrigin()->getPoint().get_y(), edge->getOrigin()->getPoint().get_z() };
	float bottomVertice[3] = { edge->get_twinEdge()->getOrigin()->getPoint().get_x() , edge->get_twinEdge()->getOrigin()->getPoint().get_y(), edge->get_twinEdge()->getOrigin()->getPoint().get_z() };

	float vdirector[3];

	vdirector[0] = topVertice[0] - bottomVertice[0];
	vdirector[1] = topVertice[1] - bottomVertice[1];
	vdirector[2] = topVertice[2] - bottomVertice[2];

	float zintersection = planes[p];
	float xintersection;
	float yintersection;

	if (vdirector[2] != 0.0)
	{
		float lambda = (zintersection - bottomVertice[2]) / vdirector[2];

		xintersection = bottomVertice[0] + lambda * vdirector[0];
		yintersection = bottomVertice[1] + lambda * vdirector[1];

		Point3D* point = new Point3D(xintersection, yintersection, zintersection);
		contour->addPoint(point);
	}
	else
	{
		std::cout << "vz from v_director in incremental slicer is 0" << std::endl;
		exit(8);
	}
}

void IncrementalSlicer::definePlanes(float thickness, Mesh_DCEL& mesh, float* delta)
{
	// planes.clear();

	// float eps = 0.004;

	// bool rounding = true; /*To avoid that the Z-coordinates of all planes are distinct from the Z-coordinates of all vertices.*/

  	// /* Assuming the model as a 3D axis-aligned bounding-box: */
  	// double model_zmax = std::max(mesh.getUpperRightVertex().get_z(), mesh.AABBSize().get_z());

  	// double model_zmin = mesh.getBottomLeftVertex().get_z();

	// double spacing = (rounding ? xround (thickness, eps, 2, 0) : thickness); /*Plane spacing even multiple of {eps}*/

	// double P0 = xround (spacing/2.f, eps, 2, 1); /*First plane odd multiple of {eps}.*/

	// int no_planes = 1 + (int)((model_zmax + spacing - P0)/spacing); /* Number of planes: */

	// for (size_t i = 0; i < no_planes; i++) {
	// /* Building the vector with the slice z coordinates: */
	// 	float Pi = (float)(P0 + i * spacing);
	// 	if ((Pi > model_zmin) && (Pi < model_zmax)) {
	// 		planes.push_back ((float)(P0 + i * spacing));
	// 	}
	// }

	// std::cout << "Model zmin = " << model_zmin << ", Model zmax = " << model_zmax << ", First plane Z = " << P0 << ", Number of planes = " << no_planes << std::endl;

	float eps = 0.004;

	bool rounding = true; /*To avoid that the Z-coordinates of all planes are distinct from the Z-coordinates of all vertices.*/

	/* Assuming the model as a 3D axis-aligned bounding-box: */
	double model_zmax = std::max(mesh.getUpperRightVertex().get_z(), mesh.AABBSize().get_z());

	double model_zmin = mesh.getBottomLeftVertex().get_z();

	double spacing = (rounding ? xround (thickness, eps, 2, 0) : thickness); /*Plane spacing even multiple of {eps}*/

	double P0 = xround (model_zmin - spacing, eps, 2, 1); /*First plane odd multiple of {eps}.*/

	int no_planes = 1 + (int)((model_zmax + spacing - P0)/spacing); /* Number of planes: */

	// std::cout << "eps = " << eps << std::endl;
	// std::cout << "max thickness = " << thickness << std::endl;
	// std::cout << "rounded plane spacing spacing = " << spacing << std::endl;
	// std::cout << "model zmin = " << model_zmin << ", model zmax = " << model_zmax << ", first plane Z = " << P0 << ", number of planes = " << no_planes << std::endl;

	for (size_t i = 0; i < no_planes; i++) {
		/* Building the vector with the slice z coordinates: */
		float Pi = (float)(P0 + i * spacing);
		if ((Pi > model_zmin) && (Pi < model_zmax)) {
			planes.push_back ((float)(P0 + i * spacing));
		}
	}
	*delta = (float)(spacing);
}

float IncrementalSlicer::xround (float x, double eps, int mod, int rem) {
  	double y = round((double)x/(mod * eps));
  	double z = (y * mod + rem) * eps;
  	return (float)z;
}

void IncrementalSlicer::defineContourOrientation(SolidSlice* slice)
{
	for (int i = 0; i < slice->getContourSize(); i++)

	{

		//orientation orientationFlag = determinateOrientation (slice->getContour(i));

		//orientation actualOrientation = findOrientation(i,slice);

		slice->getContour(i)->setOrientation(findOrientation(i, slice));

		//if( orientationFlag != actualOrientation)

		//	slice->getContour(i)->reverseList();

	}


	for (int i = 0; i < slice->getContourSize(); i++)

	{


		orientation orientationFlag = slice->getContour(i)->determinateOrientation();

		if (orientationFlag != slice->getContour(i)->getOrientation())

		{

			slice->getContour(i)->reverseList();

		}

	}

}

int IncrementalSlicer::findOrientation(int pos, SolidSlice* slice)
{
	int outsideContour = 0;

	for (int i = 0; i < slice->getContourSize(); i++)
	{
		
		if (i == pos)
			continue;

		if (pointInPolygon(slice->getContour(pos)->getPoint(0), slice->getContour(i)) == 1)
			outsideContour++;
	}

	if (outsideContour % 2 == 1)
		return ORIENTATION::CLOCKWISE;
	else return ORIENTATION::COUNTERCLOCKWISE;
}
