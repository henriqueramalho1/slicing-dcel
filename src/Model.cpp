#include "Model.h"
#include <unordered_map>
#include <array>

void Model::createModelFromMesh(Mesh_DCEL* m)
{
	meshes.emplace_back(); //There's always just one mesh in binary STLs.
	CMesh& mesh = meshes.back();

	for (const Triangle&t : m->getFaces()) {
		mesh.faces.emplace_back();
		Face& face = mesh.faces.back();
		face.vertices.push_back(t.get_v1());
		face.vertices.push_back(t.get_v2());
		face.vertices.push_back(t.get_v3());
	}
}

void Model::fill_from_model()
{

	for (const CMesh& mesh : meshes) {
		std::unordered_map<Point3D, size_t> vertex_to_index; //For each unique vertex, tracks the index within the vertex list.
		vertex_to_index.reserve(10000); //It's unknown how many unique vertices there will be and the vertices are spread around many tiny vectors, so just guess at 10k to start with.

		vertices.reserve(10000);
		triangles.reserve(mesh.faces.size()); //Would be correct if all faces are triangles. If not, it'll need to reserve more, but for most models this would be fine.

		for (const Face& face : mesh.faces) {
			//Each face is a triangle fan. We need to convert this into individual triangles.
			if (face.vertices.size() < 3) { //Not enough vertices to form a triangle. Lines and points are not saved.
				continue;
			}

			const Point3D first = face.vertices[0]; //As per the triangle fan, the first vertex is always repeated for each triangle.
			if (vertex_to_index.find(first) == vertex_to_index.end()) { //Not yet in our mesh. Need to create an index and store it in the vertex list.
				vertex_to_index.emplace(first, vertices.size());
				vertices.push_back(first);
			}
			Point3D last = face.vertices[1]; //As per the triangle fan, the last vertex is repeated for the next triangle.
			if (vertex_to_index.find(last) == vertex_to_index.end()) {
				vertex_to_index.emplace(last, vertices.size());
				vertices.push_back(last);
			}
			for (size_t i = 2; i < face.vertices.size(); ++i) {
				const Point3D vertex = face.vertices[i];
				if (vertex_to_index.find(vertex) == vertex_to_index.end()) {
					vertex_to_index.emplace(vertex, vertices.size());
					vertices.push_back(vertex);
				}
				std::array<size_t, 3> triangle = { vertex_to_index[first], vertex_to_index[last], vertex_to_index[vertex] };
				triangles.push_back(triangle);
				last = vertex; //The new last vertex.
			}
		}
	}

}

std::vector<Point3D>& Model::getVertices()
{
	return vertices;
}

std::vector<std::array<size_t, 3>>& Model::getTriangles()
{
	return triangles;
}
