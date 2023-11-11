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

void IncrementalSlicer::slice_mesh(Mesh_DCEL& mesh, float layer_thickness, std::vector<SolidSlice> &sliceList)
{
	float delta = 0.0f;

	define_planes(layer_thickness, mesh, &delta);

	std::vector<Triangle>& m = mesh.get_faces();

	Mesh_Triangle_List_t** L = build_buckets(planes, m, delta);

	Mesh_Triangle_List_t* A = new Mesh_Triangle_List_t;

	for (size_t p = 0; p < planes.size(); p++) {
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

		/*In case there is no triangle intersected by plane P[p]*/
		if (A->get_head() == NULL)
		{
			continue;
		}

		HalfEdge* itr = A->get_head()->get_t()->get_boundary();

		SolidSlice slice;

		aux = A->get_head();

		/*Main slice loop*/
		do
		{
			Mesh_Triangle_Node_t* next = aux->get_next();

			if (aux->get_t()->get_state() == false) {

				SolidContour contour;

				itr = aux->get_t()->get_boundary();

				if(create_countours(itr, &contour, p))
					slice.add_contour(contour);
			}
			aux = next;
		} while (aux != NULL);

		slice.set_z(planes[p]);
		orient(&slice);
		sliceList.push_back(slice);

		/*Reset the flag from visited triangles*/
		aux = A->get_head();
		while (aux != NULL) {
			Mesh_Triangle_Node_t* next = aux->get_next();
			aux->get_t()->reset();
			aux = next;
		}

	}
	free(L);
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

bool IncrementalSlicer::create_countours(HalfEdge* itr, SolidContour* contour, int p)
{
	HalfEdge* const start = itr;
	HalfEdge* next;
	HalfEdge* prev;

	do
	{
		next = itr->get_next_edge();
		prev = itr->get_previous_edge();

		if(next == nullptr || prev == nullptr)
		{
			return false;
		}

		if ((next->get_origin()->get_point().get_z() < planes[p] && next->get_twin_edge()->get_origin()->get_point().get_z() > planes[p])
			||
			(next->get_origin()->get_point().get_z() > planes[p] && next->get_twin_edge()->get_origin()->get_point().get_z() < planes[p]))
		{
			compute_intersection(contour, next, p);

			if (itr->get_face() != NULL)
			{
				itr->get_face()->set();
			}

			itr = next;
		}
		else if ((prev->get_origin()->get_point().get_z() < planes[p] && prev->get_twin_edge()->get_origin()->get_point().get_z() > planes[p])
			||
			(prev->get_origin()->get_point().get_z() > planes[p] && prev->get_twin_edge()->get_origin()->get_point().get_z() < planes[p]))
		{
			compute_intersection(contour, prev, p);

			if (itr->get_face() != NULL)
			{
				itr->get_face()->set();
			}

			itr = prev;

		}
		else
		{
			return false;
		}

		itr = itr->get_twin_edge();
		intersections++;

		bool ending = false;
		if(itr == start || itr->get_next_edge() == start || itr->get_twin_edge() == start || itr->get_previous_edge() == start)
		{
			itr = start;
			ending = true;
		}

		if(ending)
		{
			Point3D init_point(contour->get_points()[0]);
			contour->add_point(init_point);
		}

	} while (itr != start);

	return true;
}

void IncrementalSlicer::compute_intersection(SolidContour* contour, HalfEdge* edge, int p)
{
	float topVertice[3] = { edge->get_origin()->get_point().get_x() , edge->get_origin()->get_point().get_y(), edge->get_origin()->get_point().get_z() };
	float bottomVertice[3] = { edge->get_twin_edge()->get_origin()->get_point().get_x() , edge->get_twin_edge()->get_origin()->get_point().get_y(), edge->get_twin_edge()->get_origin()->get_point().get_z() };

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

		Point3D point(xintersection, yintersection, zintersection);
		contour->add_point(point);
	}
	else
	{
		std::cout << "vz from v_director in incremental slicer is 0" << std::endl;
		exit(8);
	}
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

void IncrementalSlicer::orient(SolidSlice* slice)
{
	for(int i = 0; i < slice->contour_number(); i++)
	{
		define_orientation(i, slice);
	}

	for(int i = 0; i < slice->contour_number(); i++)
	{
		if(slice->get_contour(i).get_orientation() == ORIENTATION::CW)
			slice->get_contour(i).reverse_point_list();
	}
}

void IncrementalSlicer::define_orientation(int pos, SolidSlice* slice)
{
	int insideContour = 0;

	for(int i = 0; i < slice->contour_number(); i++)
	{
		if (i == pos)
			continue;
		
		if(PIP(slice->get_contour(i), slice->get_contour(pos).get_point(0)))
			insideContour++;
	}

	slice->get_contour(pos).set_orientation(insideContour % 2 == 1 ? ORIENTATION::CW : ORIENTATION::CCW);
}

bool IncrementalSlicer::PIP(SolidContour c, Point3D p)
{
	int intersectionsN = 0;
    int n = c.get_size();

    for (int i = 0; i < n; i++) {
        const Point3D p1 = c.get_point(i);
        const Point3D p2 = c.get_point((i + 1) % n);

        if ((p.get_y() > p1.get_y()) != (p.get_y() > p2.get_y())) {
            double intersection_x = (p.get_y() - p1.get_y()) * (p2.get_x() - p1.get_x()) / (p2.get_y() - p1.get_y()) + p1.get_x();
            if (p.get_x() <= intersection_x) {
                intersectionsN++;
            }
        }
    }

    return intersectionsN % 2 == 1;
}