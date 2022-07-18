#include "MeshParser.h"

MeshParser::MeshParser(void)
{
}

MeshParser::~MeshParser(void)
{ }


MeshParser::MeshParser(std::string f)
{
	file = f;
}

void MeshParser::buildMesh(Mesh_DCEL* m, Model& model)
{
	model.fill_from_model();
	std::vector<Point3D> vertices = model.getVertices();
	std::vector<std::array<size_t, 3>> triangles = model.getTriangles();

	float n0, n1, n2;

	int n_faces = triangles.size();

	/*std::cout << "Parsing file" << std::endl;
	std::cout << "Triangles size: " << triangles.size() << std::endl;
	std::cout << "Vertices size: " << vertices.size() << std::endl;*/

	int n_vertices = vertices.size();
	m->getVertices().reserve(n_vertices);

	for (int i = 0; i < n_vertices; i++)
	{
		int id = m->createVertex();
		m->getVertex(id)->setPoint(vertices[i]);
	}

	m->getFaces().reserve(n_faces);
	m->getHalfEdges().reserve(2 * (3 * n_faces)); // just a good expensive number of edges =)

	for (int i = 0; i < n_faces; i++) //store the triangles
	{

		std::array<size_t, 3> points = triangles[i];
		n0 = points[0];
		n1 = points[1];
		n2 = points[2];

		//std::cout << "index: " << i << " ";
		int fid = m->createTriangularFace(n0, n1, n2);
		if (fid != -1)
		{
			m->checkFace(fid);
		}
	}

	m->manageUnhandledTriangles();
	m->checkAllFaces();
}
