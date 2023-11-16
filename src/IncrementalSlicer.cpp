#include "IncrementalSlicer.h"
#include <iostream>
#include <chrono>
#include "HalfEdge.h"

IncrementalSlicer::IncrementalSlicer():
	intersections(0)
{

}

IncrementalSlicer::~IncrementalSlicer()
{
	
}

std::vector<SolidSlice> IncrementalSlicer::slice_mesh(Mesh_DCEL& mesh, float layer_thickness)
{
	float delta = 0.0f;

	define_planes(layer_thickness, mesh, &delta);

	std::vector<HalfEdge>& h = mesh.get_halfedges();

	HalfEdgeList** L = build_edges_buckets(planes, h, delta);

	// for(int i = 0; i < planes.size(); i++)
	// {
	// 	std::cout << "Bucket "<< i << "size = " << L[i]->size() << std::endl;
	// }

	HalfEdgeList A;

	int analyzing_layer = 117;

	std::vector<SolidSlice> slice_list;

	for (size_t p = 0; p < planes.size(); p++) 
	{
		SolidSlice slice;
		HalfEdgeList B;
		A.join(L[p]);

		// if(p == analyzing_layer)
		// {
		// 	std::cout << "Iteracao " << p << " A com tamanho "<< A.size() << std::endl;
		// }
	
		while (!A.is_empty())
		{
			HalfEdgeNode* halfedge_node = A.get_head();

			bool extracted = false;

			if (halfedge_node->get_h()->get_destiny_z() < planes[p])
				A.remove(halfedge_node);

			else
			{
				// if (p == analyzing_layer)
				// {
				// 	extracted = true;
				// 	std::cout << "Comecando a extrair e tamanho de A = " << A.size() << std::endl;
				// }
				std::cout << "Extracting from p " << p << std::endl;
				SolidContour contour = extract_contour(halfedge_node->get_h(), &A, &B, planes[p]);
				slice.add_contour(contour);
			}

			// if(p == analyzing_layer && extracted)
			// {
			// 	std::cout << "A size = " << A.size() << std::endl;
			// 	std::cout << "B size = " << B.size() << std::endl;
			// }
		}
		
		// std::cout << "Slice["<< p <<"] size = " << slice.contour_number() << std::endl;

		// if(p == analyzing_layer)
		// 	exit(2);

		A = B;
		slice_list.push_back(slice);

	}

	delete L;

	return slice_list;
}

Point3D IncrementalSlicer::intersection(HalfEdge *h, float z)
{
	Point3D origin = h->get_origin()->get_point();
	Point3D destiny = h->get_twin_edge()->get_origin()->get_point();

    float s = (z - origin.get_z()) / (destiny.get_z() - origin.get_z());
	float r = 1 - s;

	float xf = r * origin.get_x() + s * destiny.get_x();
	float yf = r * origin.get_y() + s * destiny.get_y();

	return Point3D(xf, yf, z);
}

Mesh_Triangle_List_t** IncrementalSlicer::build_buckets(std::vector<float>& planes, std::vector<Triangle>& mesh, float delta)
{
	size_t k = planes.size();
	Mesh_Triangle_List_t** L = (Mesh_Triangle_List_t**)malloc((k + 1) * sizeof(Mesh_Triangle_List_t*));

	for (size_t p = 0; p <= k; p++) {
		L[p] = new Mesh_Triangle_List_t;
	}

	size_t n = mesh.size(); /* Number of triangles. */

	if (delta > 0.0) {
		for (size_t i = 0; i < n; ++i) {
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
		for (size_t i = 0; i < n; ++i) {
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

HalfEdgeList **IncrementalSlicer::build_edges_buckets(std::vector<float> &planes, std::vector<HalfEdge> &halfedges, float delta)
{
    size_t k = planes.size();
	HalfEdgeList** L = (HalfEdgeList**)malloc((k + 1) * sizeof(HalfEdgeList*));

	for (size_t p = 0; p <= k; p++) {
		L[p] = new HalfEdgeList;
	}

	size_t n = halfedges.size(); /* Number of halfedges. */

	if (delta > 0.0) {
		for (size_t i = 0; i < n; ++i) {
			HalfEdge* h = &halfedges[i];
			int p;
			if (h->get_zmin() < planes[0]) {
				p = 0;
			}
			else if (h->get_zmin() > planes[k-1]) {
				p = k;
			}
			else {
				p = floor((h->get_zmin() - planes[0])/delta) + 1;
			}
			L[p]->insert(h);
		}
	}
	else {
		for (size_t i = 0; i < n; ++i) {
			HalfEdge* h = &halfedges[i];
			//Binary search
			/* Assumes that {planes} is a list of {k} strictly increasing {Z} coordinates.
			Returns an integer {p} such that {planes[p-1] < zMin < planes[p]}. As special cases,
			if {zMin < planes[0]} returns 0; if {zMin > planes[k-1]} returns {k}. */
			int p;
			if (h->get_zmin() >= planes[k - 1]) { p = k; }
			int l = -1; /* Inferior Z index. */
			int r = k;  /* Superior Z index. */
			while (r - l > 1) {
				/* At this point, {zMin} is between {P[l]} and {P[r]}. */
				int m = (l + r) / 2;
				if (h->get_zmin() >= planes[m]) {
					l = m;
				}
				else {
					r = m;
				}
			}
			p = r;
			// end Binary search
			L[p]->insert(h);
		}
	}
	return L;
}

SolidContour IncrementalSlicer::extract_contour(HalfEdge *h, HalfEdgeList *A, HalfEdgeList *B, float z)
{
	HalfEdge* init_h = h;
	SolidContour contour;
	//std::cout << "Initiating contour from z = " << z << std::endl;

    do
	{
		h = h->get_twin_edge()->get_next_edge();
		while(h->get_destiny_z() < z)
			h = h->get_next_edge();

		Point3D p = intersection(h, z);
		contour.add_point(p);
		intersections++;

		if(A->size() == 0)
		{
			std::cout << "A size == 0" << std::endl;
			exit(2);
		}

		A->remove(h->get_node());
		B->insert(h);

	} while (h != init_h && h->get_twin_edge() != init_h && h->get_next_edge() != init_h && h->get_previous_edge() != init_h);
	
	contour.set_orientation(orient(contour));
	return contour;
}

void IncrementalSlicer::define_planes(float thickness, Mesh_DCEL& mesh, float* delta)
{
	float eps = 0.004;

	bool rounding = true; /*To avoid that the Z-coordinates of all planes are distinct from the Z-coordinates of all vertices.*/

	/* Assuming the model as a 3D axis-aligned bounding-box: */
	double model_zmax = std::max(mesh.get_upper_right_vertex().get_z(), mesh.AABB_size().get_z());

	double model_zmin = mesh.get_bottom_left_vertex().get_z();

	double spacing = (rounding ? xround (thickness, eps, 2, 0) : thickness); /*Plane spacing even multiple of {eps}*/

	double P0 = xround (model_zmin - spacing, eps, 2, 1); /*First plane odd multiple of {eps}.*/

	int no_planes = 1 + (int)((model_zmax + spacing - P0)/spacing); /* Number of planes: */

	// std::cout << "eps = " << eps << std::endl;
	// std::cout << "max thickness = " << thickness << std::endl;
	// std::cout << "rounded plane spacing = " << spacing << std::endl;
	// std::cout << "P0 = " << P0 << std::endl;
	// std::cout << "model zmin = " << model_zmin << ", model zmax = " << model_zmax << ", first plane Z = " << P0 << ", number of planes = " << no_planes << std::endl;

	for (int i = 0; i < no_planes; i++) {
		/* Building the vector with the slice z coordinates: */
		float Pi = (float)(P0 + i * spacing);
		if ((Pi > model_zmin) && (Pi < model_zmax)) {
			planes.push_back ((float)(P0 + i * spacing));
		}
	}
	*delta = (float)(spacing);
}

float IncrementalSlicer::xround (float x, double eps, int mod, int rem) {
  	float y = roundf((float)(x/(mod * eps)));
  	double z = (double)(y * mod + rem) * eps;

  	return (float)z;
}

ORIENTATION IncrementalSlicer::orient(SolidContour contour)
{
	// Calcula o produto vetorial dos vetores que ligam os pontos
	int orientation = 0;
	for (int i = 0; i < contour.get_size(); i++) 
	{
		orientation += contour.get_point((i + 1) % contour.get_size()).get_x() * contour.get_point((i + 2) % contour.get_size()).get_y() - contour.get_point((i + 1) % contour.get_size()).get_y() * contour.get_point((i + 2) % contour.get_size()).get_x();
	}

 	// Se o produto vetorial for positivo, a lista está orientada CW
  	// Se o produto vetorial for negativo, a lista está orientada CCW

	if (orientation < 0)
		return ORIENTATION::CW;

  	return ORIENTATION::CCW;
}