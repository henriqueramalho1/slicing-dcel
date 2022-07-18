#pragma once

#include <vector>
#include <array>
#include "Conversion_Mesh.h"
#include "Mesh_DCEL.h"
#include "Point3D.h"

class Model {
	public:
	
	std::vector<CMesh> meshes;

	std::vector<Point3D> vertices;

	std::vector<std::array<size_t, 3>> triangles;

	void createModelFromMesh(Mesh_DCEL* m);

	void fill_from_model();

	std::vector<Point3D>& getVertices();

	std::vector<std::array<size_t, 3>>& getTriangles();
};
