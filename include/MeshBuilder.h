#pragma once
#include "Mesh_DCEL.h"
#include <CGAL/Surface_mesh.h>
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/IO/io.h>
#include <CGAL/HalfedgeDS_default.h>
#include <CGAL/Polygon_mesh_processing/IO/polygon_mesh_io.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel  Kernel;
typedef CGAL::Surface_mesh<Kernel::Point_3>                      SurfaceMesh;

class MeshBuilder
{
private:
	SurfaceMesh mesh;
	Mesh_DCEL* dcel;

public:
	MeshBuilder(Mesh_DCEL* m);
	~MeshBuilder();
	void build(std::string path);
	void clone();
};

