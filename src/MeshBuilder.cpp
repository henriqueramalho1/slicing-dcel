#include "MeshBuilder.h"
#include "Vertex.h"



MeshBuilder::MeshBuilder(Mesh_DCEL* m):
	mesh(),
	dcel(m)
{
	
}

MeshBuilder::~MeshBuilder()
{
}

void MeshBuilder::build(std::string path)
{
	if (CGAL::IO::read_polygon_mesh(path, mesh))
	{
		std::cout << "Malha aberta com sucesso" << std::endl;
	} 
	std::cout << "Edges Size: " << mesh.halfedges().size() << std::endl;
	std::cout << "Faces Size: " << mesh.faces().size() << std::endl;
	std::cout << "Vertices Size: " << mesh.vertices().size() << std::endl;

	clone();
}

void MeshBuilder::clone()
{
	std::vector<Triangle>& dcel_faces = dcel->getFaces();
	std::vector<halfEdge>& dcel_halfedges = dcel->getHalfEdges();
	std::vector<Vertex>& dcel_vertices = dcel->getVertices();
	std::vector<std::vector<int>>& dcel_face_indices = dcel->getFaceIndices();

	for (SurfaceMesh::Vertex_index vi : mesh.vertices())
	{
		Kernel::Point_3 pt = mesh.point(vi);
		dcel_vertices.push_back(Vertex(vi.idx(), Point3D((float)pt.x(), (float)pt.y(), (float)pt.z())));
		dcel->setUpperBottomVertices(dcel_vertices[vi.idx()].getPoint());
	}

	for (SurfaceMesh::Edge_index edge_index : mesh.edges())
	{
		dcel_halfedges.push_back(halfEdge(edge_index.halfedge().idx(), &dcel_vertices[source(edge_index.halfedge(), mesh).idx()]));
		dcel_halfedges.push_back(halfEdge(opposite(edge_index.halfedge(), mesh).idx(), &dcel_vertices[source(opposite(edge_index.halfedge(), mesh), mesh).idx()]));
	}

	for (SurfaceMesh::Face_index face_index : mesh.faces())
	{
		//std::cout << face_index << "  // Boundary" << mesh.halfedge(face_index).idx() << std::endl;
		dcel_face_indices.emplace_back();
		CGAL::Vertex_around_face_circulator<SurfaceMesh> vcirc(mesh.halfedge(face_index), mesh), done(vcirc);
		std::vector<int>& ind = dcel_face_indices.back();
		do
		{
			ind.push_back(*vcirc++);
		} while (vcirc != done);

		dcel_faces.push_back(Triangle(nullptr, &dcel_vertices[ind[0]], &dcel_vertices[ind[1]], &dcel_vertices[ind[2]]));
	}
	

	for (SurfaceMesh::Face_index face_index : mesh.faces())
	{
		SurfaceMesh::Halfedge_index halfedge_index = mesh.halfedge(face_index);

		dcel_faces[face_index.idx()].set_boundary(&dcel_halfedges[mesh.halfedge(face_index).idx()]);

		dcel_halfedges[halfedge_index.idx()].set_Face(&dcel_faces[face_index.idx()]);
		dcel_halfedges[halfedge_index.idx()].set_nextEdge(&dcel_halfedges[next(halfedge_index, mesh).idx()]);
		dcel_halfedges[halfedge_index.idx()].set_previousEdge(&dcel_halfedges[prev(halfedge_index, mesh).idx()]);
		dcel_halfedges[halfedge_index.idx()].set_twinEdge(&dcel_halfedges[opposite(halfedge_index, mesh).idx()]);

		halfedge_index = next(halfedge_index, mesh);

		dcel_halfedges[halfedge_index.idx()].set_Face(&dcel_faces[face_index.idx()]);
		dcel_halfedges[halfedge_index.idx()].set_nextEdge(&dcel_halfedges[next(halfedge_index, mesh).idx()]);
		dcel_halfedges[halfedge_index.idx()].set_previousEdge(&dcel_halfedges[prev(halfedge_index, mesh).idx()]);
		dcel_halfedges[halfedge_index.idx()].set_twinEdge(&dcel_halfedges[opposite(halfedge_index, mesh).idx()]);

		halfedge_index = next(halfedge_index, mesh);

		dcel_halfedges[halfedge_index.idx()].set_Face(&dcel_faces[face_index.idx()]);
		dcel_halfedges[halfedge_index.idx()].set_nextEdge(&dcel_halfedges[next(halfedge_index, mesh).idx()]);
		dcel_halfedges[halfedge_index.idx()].set_previousEdge(&dcel_halfedges[prev(halfedge_index, mesh).idx()]);
		dcel_halfedges[halfedge_index.idx()].set_twinEdge(&dcel_halfedges[opposite(halfedge_index, mesh).idx()]);
	}

	dcel->roundVertices();

	std::cout << "Faces: " << dcel_faces.size() << std::endl;
	std::cout << "Halfedges: " << dcel_halfedges.size() << std::endl;
	std::cout << "Vertices: " << dcel_vertices.size() << std::endl;
}
