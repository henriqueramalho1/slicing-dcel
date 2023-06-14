#pragma once
#include "Mesh_DCEL.h"
#include <CGAL/Surface_mesh.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/IO/io.h>
#include <CGAL/HalfedgeDS_default.h>
#include <CGAL/Polygon_mesh_processing/IO/polygon_mesh_io.h>

//typedef CGAL::Simple_cartesian<double>     Kernel;
typedef CGAL::Exact_predicates_inexact_constructions_kernel  Kernel;
typedef CGAL::Surface_mesh<Kernel::Point_3>                      SurfaceMesh;
typedef CGAL::Polyhedron_3<Kernel>                  Polyhedron;

class MeshBuilder
{
private:
	SurfaceMesh mesh;
	Polyhedron mesh2;
	Mesh_DCEL* dcel;

public:
	MeshBuilder(Mesh_DCEL* m);
	~MeshBuilder();
	bool build(std::string path);
	void clone();
};

