#include "Mesh_DCEL.h"


Mesh_DCEL::Mesh_DCEL():
upperRightVertex(),
bottomLeftVertex()
{
	unhandledTrianglesCount = 0;
}

Mesh_DCEL::~Mesh_DCEL()
{
	this->clear();
}

std::list<int> Mesh_DCEL::getUnhandledTriangles() const
{
	return this->unhandledTriangles;
}

void Mesh_DCEL::manageUnhandledTriangles()
{
	if (unhandledTrianglesCount * 3 != unhandledTriangles.size())
	{
		std::cout << "The number of unhandled triangles should be equal to the number of unhandled vertices*3!";
	}

	unsigned int curTriangle = 0;
	while (!this->unhandledTriangles.empty())
	{
		if (curTriangle == unhandledTrianglesCount)
		{
			unsigned int remainingTriangles = this->unhandledTriangles.size() / 3;
			if (remainingTriangles < unhandledTrianglesCount)
			{
				unhandledTrianglesCount = remainingTriangles;
				curTriangle = 0;
			}
			else
			{
				std::cout << "There are triangles that cannot be added to the mesh!" << std::endl;
			}
		}

		//std::cout << "Preparing to access unhandledTriangles vector" << std::endl;

		int v1 = unhandledTriangles.front();
		unhandledTriangles.pop_front();
		int v2 = unhandledTriangles.front();
		unhandledTriangles.pop_front();
		int v3 = unhandledTriangles.front();
		unhandledTriangles.pop_front();
		unsigned int fid = this->createTriangularFace(v1, v2, v3);

		//std::cout << "unhandledTriangles vector accessed" << std::endl;

		if (fid == -1) //ajust the number of triangles in case of the triangle was not added to the mesh
		{
			unhandledTrianglesCount--;
		}

		curTriangle++;
	}

	std::cout << "Unhandled Triangles Managed" << std::endl;
}

unsigned int Mesh_DCEL::createFace(halfEdge* bound, Vertex* v1, Vertex* v2, Vertex* v3)
{
	unsigned int faceId = this->faces.size();
	this->faces.push_back(Triangle(bound, v1, v2, v3));
	Triangle* face = &(faces[faceId]);
	face->set_boundary(bound);
	return faceId;
}

unsigned int Mesh_DCEL::createTriangularFace(unsigned int vId1, unsigned int vId2, unsigned int vId3)
{
	Vertex* v1 = this->getVertex(vId1);
	Vertex* v2 = this->getVertex(vId2);
	Vertex* v3 = this->getVertex(vId3);

	//adicionar pontos extremos
	setUpperBottomVertices(v1->getPoint());
	setUpperBottomVertices(v2->getPoint());
	setUpperBottomVertices(v3->getPoint());

	halfEdge* e1 = this->getHalfEdge(v1, v2);
	halfEdge* e2 = this->getHalfEdge(v2, v3);
	halfEdge* e3 = this->getHalfEdge(v3, v1);

	int unusedVertices = 0;
	if (v1->getIncidentEdge() == NULL) unusedVertices++;
	if (v2->getIncidentEdge() == NULL) unusedVertices++;
	if (v3->getIncidentEdge() == NULL) unusedVertices++;

	int readyEdges = 0;
	if (e1 != NULL) readyEdges++;
	if (e2 != NULL) readyEdges++;
	if (e3 != NULL) readyEdges++;

	unsigned int nullFaceId = MESH_NULL_ID;
	unsigned int faceId = nullFaceId;
	Triangle* face = NULL;

	//the most simple case, all vertices has degree 0
	//create 3 edges, and link then
	if (unusedVertices == 3 && readyEdges == 0)
	{

		//std::cout << "If1" << std::endl;

		faceId = createFace(NULL, v1, v2, v3);
		face = getFace(faceId);
		 
		e1 = getHalfEdge(createEdge(v1, face, v2, NULL));
		v1->setIncidentEdge(e1);
		face->set_boundary(e1);

		e2 = getHalfEdge(createEdge(v2, face, v3, NULL));
		v2->setIncidentEdge(e2);

		e3 = getHalfEdge(createEdge(v3, face, v1, NULL));
		v3->setIncidentEdge(e3);

		e1->set_nextEdge(e2);
		e2->set_nextEdge(e3);
		e3->set_nextEdge(e1);

		e1->get_twinEdge()->set_nextEdge(e3->get_twinEdge());
		e3->get_twinEdge()->set_nextEdge(e2->get_twinEdge());
		e2->get_twinEdge()->set_nextEdge(e1->get_twinEdge());
	}

	//there are one vertex that has been used by another triangle.
//Create the 3 edges, and link to the old triangle.
	else if (unusedVertices == 2 && readyEdges == 0)
	{
		//std::cout << "If2" << std::endl;

		if (v2->getIncidentEdge() != NULL)
		{
			Vertex* vt = v1;
			v1 = v2;
			v2 = v3;
			v3 = vt;
		}
		else if (v3->getIncidentEdge() != NULL)
		{
			Vertex* vt = v1;
			v1 = v3;
			v3 = v2;
			v2 = vt;
		}
		halfEdge* ei1 = findIncidentalHalfEdge(v1);
		if (ei1 != NULL)
		{
			halfEdge* ei1n = ei1->get_nextEdge();

			faceId = createFace(NULL, v1, v2, v3);
			face = getFace(faceId);

			e1 = getHalfEdge(createEdge(v1, face, v2, ei1->get_Face()));
			face->set_boundary(e1);

			e2 = getHalfEdge(createEdge(v2, face, v3, ei1->get_Face()));
			v2->setIncidentEdge(e2);

			e3 = getHalfEdge(createEdge(v3, face, v1, ei1->get_Face()));
			v3->setIncidentEdge(e3);

			e1->set_nextEdge(e2);
			e2->set_nextEdge(e3);
			e3->set_nextEdge(e1);

			ei1->set_nextEdge(e3->get_twinEdge());
			e3->get_twinEdge()->set_nextEdge(e2->get_twinEdge());
			e2->get_twinEdge()->set_nextEdge(e1->get_twinEdge());
			e1->get_twinEdge()->set_nextEdge(ei1n);
		}
	}
	//2 vertices are already in use by 2 distinct triangles, and
//one vertex doesn't have been used yet.
	else if (unusedVertices == 1 && readyEdges == 0)
	{
		//std::cout << "If3" << std::endl;

		if (v3->getIncidentEdge() == NULL)
		{
			Vertex* vt = v1;
			v1 = v2;
			v2 = v3;
			v3 = vt;
		}
		else if (v1->getIncidentEdge() == NULL)
		{
			Vertex* vt = v1;
			v1 = v3;
			v3 = v2;
			v2 = vt;
		}

		halfEdge* ei1 = findIncidentalHalfEdge(v1);
		halfEdge* ei3 = findIncidentalHalfEdge(v3);

		if (ei1 != NULL && ei3 != NULL)
		{
			halfEdge* ei1n = ei1->get_nextEdge();
			halfEdge* ei3n = ei3->get_nextEdge();

			faceId = createFace(NULL, v1, v2, v3);
			face = getFace(faceId);

			e1 = getHalfEdge(createEdge(v1, face, v2, ei1n->get_Face()));
			e2 = getHalfEdge(createEdge(v2, face, v3, ei3->get_Face()));
			e3 = getHalfEdge(createEdge(v3, face, v1, ei1->get_Face()));

			face->set_boundary(e1);
			v2->setIncidentEdge(e2);

			e1->set_nextEdge(e2);
			e2->set_nextEdge(e3);
			e3->set_nextEdge(e1);

			ei1->set_nextEdge(e3->get_twinEdge());
			e3->get_twinEdge()->set_nextEdge(ei3n);

			ei3->set_nextEdge(e2->get_twinEdge());
			e2->get_twinEdge()->set_nextEdge(e1->get_twinEdge());
			e1->get_twinEdge()->set_nextEdge(ei1n);
		}
	}

	//two of the vertices already have one edge connected between then,
//and one vertex has not been used yet.
	else if (unusedVertices == 1 && readyEdges == 1)
	{
		//std::cout << "If4" << std::endl;

		//rotate pointers, so the v1 and v2 are connected, and v3 is unused
		if (e2 != NULL)
		{
			Vertex* vt = v1;
			v1 = v2;
			v2 = v3;
			v3 = vt;
			halfEdge* et = e1;
			e1 = e2;
			e2 = e3;
			e3 = et;
		}
		else if (e3 != NULL)
		{
			Vertex* vt = v1;
			v1 = v3;
			v3 = v2;
			v2 = vt;
			halfEdge* et = e1;
			e1 = e3;
			e3 = e2;
			e2 = et;
		}

		if (e1->get_Face() == NULL)
		{
			halfEdge* e1n = e1->get_nextEdge();
			halfEdge* e1p = e1->get_previousEdge();

			faceId = createFace(e1, v1, v2, v3);
			face = getFace(faceId);

			e2 = getHalfEdge(createEdge(v2, face, v3, e1n->get_Face()));
			e3 = getHalfEdge(createEdge(v3, face, v1, e1p->get_Face()));

			e1->set_Face(face);
			v3->setIncidentEdge(e3);

			e1->set_nextEdge(e2);
			e2->set_nextEdge(e3);
			e3->set_nextEdge(e1);

			e1p->set_nextEdge(e3->get_twinEdge());
			e3->get_twinEdge()->set_nextEdge(e2->get_twinEdge());
			e2->get_twinEdge()->set_nextEdge(e1n);
		}
	}

	//all the tree vertices are used by different triangles, no 
	//ready edge available.
	else if (unusedVertices == 0 && readyEdges == 0)
	{
		//std::cout << "If5" << std::endl;

		halfEdge* ei1 = findIncidentalHalfEdge(v1);
		halfEdge* ei2 = findIncidentalHalfEdge(v2);
		halfEdge* ei3 = findIncidentalHalfEdge(v3);

		if (ei1 != NULL && ei2 != NULL && ei3 != NULL)
		{

			halfEdge* ei1n = ei1->get_nextEdge();
			halfEdge* ei2n = ei2->get_nextEdge();
			halfEdge* ei3n = ei3->get_nextEdge();

			faceId = createFace(NULL, v1, v2, v3);
			face = getFace(faceId);

			e1 = getHalfEdge(createEdge(v1, face, v2, ei2->get_Face()));
			e2 = getHalfEdge(createEdge(v2, face, v3, ei3->get_Face()));
			e3 = getHalfEdge(createEdge(v3, face, v1, ei1->get_Face()));

			face->set_boundary(e1);

			e1->set_nextEdge(e2);
			e2->set_nextEdge(e3);
			e3->set_nextEdge(e1);

			ei1->set_nextEdge(e3->get_twinEdge());
			e3->get_twinEdge()->set_nextEdge(ei3n);

			ei3->set_nextEdge(e2->get_twinEdge());
			e2->get_twinEdge()->set_nextEdge(ei2n);

			ei2->set_nextEdge(e1->get_twinEdge());
			e1->get_twinEdge()->set_nextEdge(ei1n);
		}
	}

	//all vertices are used, and two of then are used by the 
	//same triangle. In this case, one edge will be shared with
	//other triangle.
	else if (unusedVertices == 0 && readyEdges == 1)
	{
		//std::cout << "If6" << std::endl;
		if (e2 != NULL)
		{
			Vertex* vt = v1;
			v1 = v2;
			v2 = v3;
			v3 = vt;
			halfEdge* et = e1;
			e1 = e2;
			e2 = e3;
			e3 = et;
		}
		else if (e3 != NULL)
		{
			Vertex* vt = v1;
			v1 = v3;
			v3 = v2;
			v2 = vt;
			halfEdge* et = e1;
			e1 = e3;
			e3 = e2;
			e2 = et;
		}

		halfEdge* ei3 = findIncidentalHalfEdge(v3);
		if (ei3 && e1->get_Face() == NULL)
		{
			halfEdge* e1p = e1->get_previousEdge();
			halfEdge* e1n = e1->get_nextEdge();
			halfEdge* ei3n = ei3->get_nextEdge();

			faceId = createFace(e1, v1, v2, v3);
			face = getFace(faceId);

			e2 = getHalfEdge(createEdge(v2, face, v3, ei3->get_Face()));
			e3 = getHalfEdge(createEdge(v3, face, v1, e1p->get_Face()));

			e1->set_Face(face);
			e2->set_Face(face);
			e3->set_Face(face);

			e1->set_nextEdge(e2);
			e2->set_nextEdge(e3);
			e3->set_nextEdge(e1);

			e1p->set_nextEdge(e3->get_twinEdge());
			e3->get_twinEdge()->set_nextEdge(ei3n);

			ei3->set_nextEdge(e2->get_twinEdge());
			e2->get_twinEdge()->set_nextEdge(e1n);
		}
	}

	//The three vertices are used, but there is an edge missing.
	//will create one edge to 'fill' the hole and expand the mesh
	else if (unusedVertices == 0 && readyEdges == 2)
	{
		//std::cout << "If7" << std::endl;
		if (e1 == NULL)
		{
			Vertex* vt = v1;
			v1 = v3;
			v3 = v2;
			v2 = vt;
			halfEdge* et = e1;
			e1 = e3;
			e3 = e2;
			e2 = et;
		}
		else if (e3 == NULL)
		{
			Vertex* vt = v1;
			v1 = v2;
			v2 = v3;
			v3 = vt;
			halfEdge* et = e1;
			e1 = e2;
			e2 = e3;
			e3 = et;
		}

		if (e1->get_Face() == NULL && e3->get_Face() == NULL)
		{
			halfEdge* e1n = e1->get_nextEdge();
			halfEdge* e3p = e3->get_previousEdge();

			if (e3->get_nextEdge() == e1 && e1->get_previousEdge() == e3)
			{
				faceId = createFace(e1, v1, v2, v3);
				face = getFace(faceId);

				e2 = getHalfEdge(createEdge(v2, face, v3, e1n->get_Face()));

				e1->set_Face(face);
				e3->set_Face(face);

				e1->set_nextEdge(e2);
				e2->set_nextEdge(e3);
				e3->set_nextEdge(e1);

				e3p->set_nextEdge(e2->get_twinEdge());
				e2->get_twinEdge()->set_nextEdge(e1n);
			}
			else
			{
				std::cerr << "outro erro" << std::endl;
			}
		}
	}

	//A practically closed face, need only that the internal edges
	//point to a new face. A easy case =)
	else if (unusedVertices == 0 && readyEdges == 3)
	{
		//std::cout << "If8" << std::endl;
		if (e1->get_Face() == NULL && e2->get_Face() == NULL && e3->get_Face() == NULL)
		{
			if (e1->get_nextEdge() == e2 && e2->get_nextEdge() == e3 && e3->get_nextEdge() == e1)
			{
				faceId = createFace(e1, v1, v2, v3);
				face = getFace(faceId);
				e1->set_Face(face);
				e2->set_Face(face);
				e3->set_Face(face);
			}
			else
			{
				std::cerr << "mais outro erro" << std::endl;
			}
		}
	}

	// if there is no face pointer, then a triangle should be inserted in
	// a difficult case to handle. For now, the most simple solution is
	// handle this triangle latter, after other triangles have been added
	// and there is no ambiguity
	if (face == NULL)
	{
		this->unhandledTriangles.push_back(vId1);
		this->unhandledTriangles.push_back(vId2);
		this->unhandledTriangles.push_back(vId3);
		unhandledTrianglesCount++;
	}


	return faceId;
};

std::vector<Triangle>& Mesh_DCEL::getFaces()
{
	return this->faces;
}

const std::vector<Triangle>& Mesh_DCEL::getFaces() const
{
	return this->faces;
}

unsigned int Mesh_DCEL::getNumFaces() const
{
	return this->faces.size();
}

Triangle* Mesh_DCEL::getFace(unsigned int faceId) const
{
	const Triangle* f = &(this->faces[faceId]);
	return const_cast<Triangle*>(f);
}

unsigned int Mesh_DCEL::getFaceId(const Triangle* face) const
{
	const Triangle* firstFace = &(this->faces[0]);
	unsigned int id = face - firstFace;
	if (id >= 0 && id < this->faces.size())
	{
		if (&(this->faces[id]) == face)
			return id;
	}
	return MESH_NULL_ID;
}

void Mesh_DCEL::checkAllFaces() const
{
	const unsigned int numFaces = this->getNumFaces();
	const unsigned int numEdges = this->getNumHalfEdges();

	for (unsigned int f = 0; f < numFaces; ++f)
	{
		this->checkFace(f);
	}

	/*for (unsigned int i = 0; i < numEdges; i++)
	{
		if (edges[i].get_Face() == NULL)
		{
			std::cout << "Edge " << i << " com face NULL" << std:: endl;
		}
	}*/
}

void Mesh_DCEL::checkFace(unsigned int faceId) const
{
	Triangle* face = this->getFace(faceId);

	if (face->get_boundary() == NULL)
	{
		std::cout << "Every face must have a boundary (1)";
	}

	halfEdge* edge = NULL;
	EdgeIterator it(face);
	while (it.hasNext())
	{
		edge = it.getNext();

		if (edge->get_Face() != face)
		{
			std::cout << "The sequence of half-edges pointed by face->boundary must point to the same face (2)";
		}

		if (edge->getOrigin() == NULL)
		{
			std::cout << "Every edge must have an origin vertex (3)";
		}

		if (edge->getOrigin()->getIncidentEdge() == NULL)
		{
			std::cout << "Every vertex must have a incident pointer (4)";
		}

		if (edge->get_twinEdge() == NULL)
		{
			std::cout << "Every edge must have an twin edge (5)";
		}

		if (edge->get_nextEdge() == NULL)
		{
			std::cout << "Every edge must have a next half-edge pointer (6)";
		}
		else if (edge->get_nextEdge()->getOrigin() != edge->get_twinEdge()->getOrigin())
		{
			std::cout << "The pointer edge->next->origin must be equal to edge->twin->origin (7)";
		}

		if (edge->get_previousEdge() == NULL)
		{
			std::cout << "Every edge must have a prev pointer (8)";
		}
		else if (edge->get_previousEdge()->get_twinEdge()->getOrigin() != edge->getOrigin())
		{
			std::cout << "The pointer edge->prev->twin->origin must be equal to edge->origin (9)";
		}

		bool edgeFound = false;
		unsigned int count = 0;
		EdgeIterator vit(edge->getOrigin());
		while (vit.hasNext())
		{
			if (vit.getNext() == edge)
			{
				edgeFound = true;
				break;
			}
			else
			{
				// if a "infinite" loop happens here, also there is an undetectable error!
				// lets say that a vertex will not have more than 1000 incident edges.
				count++;
				if (count > 1000)
				{
					std::cout << "There is an error iterating through the edges of a vertex - possible infinite loop - (10)";
				}
			}
		}
		if (!edgeFound)
		{
			std::cout << "The edge cannot be reachable iterating thought its origin (11)";
		}

		if (edge->get_twinEdge()->get_Face() == NULL)
		{
			const halfEdge* initialBorder = edge->get_twinEdge();
			const halfEdge* cEdge = initialBorder->get_nextEdge();
			while (cEdge != initialBorder)
			{
				if (cEdge->get_Face() != NULL)
				{
					std::cout << "The sequence of half edges on the border does not points to a null face (12)";
				}
				cEdge = cEdge->get_nextEdge();
			}
		}
	}

	if (edge->get_nextEdge() != face->get_boundary())
	{
		std::cout << "The pointer edge->next of the last edge in a face border must be equals to the face->boundary (12)";
	}
}

unsigned int Mesh_DCEL::createVertex()
{
	this->vertices.push_back(Vertex());
	return this->vertices.size() - 1;
}

Vertex* Mesh_DCEL::createGetVertex()
{
	this->vertices.push_back(Vertex());
	return &this->vertices.back();
}

Vertex* Mesh_DCEL::getVertex(unsigned int id) const
{
	const Vertex* v = &(this->vertices[id]);
	return const_cast<Vertex*>(v);
}

const std::vector<Vertex>& Mesh_DCEL::getVertices() const
{
	return this->vertices;
}

std::vector<Vertex>& Mesh_DCEL::getVertices()
{
	return this->vertices;
}

unsigned int Mesh_DCEL::getNumVertices() const
{
	return this->vertices.size();
}

unsigned int Mesh_DCEL::getVertexId(const Vertex* vertex) const
{
	const Vertex* firstVertex = &(this->vertices[0]);
	unsigned int id = vertex - firstVertex;
	if (id >= 0 && id < this->vertices.size())
	{
		if (&(this->vertices[id]) == vertex)
			return id;
	}
	return MESH_NULL_ID;
}

unsigned int Mesh_DCEL::createEdge(Vertex* origin, Triangle* face, Vertex* twinOrigin, Triangle* twinFace)
{
	unsigned int edgeId = this->edges.size();
	this->edges.push_back(halfEdge());
	halfEdge* e1 = &this->edges[edgeId];
	e1->setOrigin(origin);
	e1->set_Face(face);

	this->edges.push_back(halfEdge());
	halfEdge* e2 = &this->edges[edgeId + 1];
	e2->setOrigin(twinOrigin);
	e2->set_Face(twinFace);

	e1 = &this->edges[edgeId];
	e1->set_twinEdge(e2);

	return edgeId;
}

std::vector<halfEdge>& Mesh_DCEL::getHalfEdges()
{
	return this->edges;
}

const std::vector<halfEdge>& Mesh_DCEL::getHalfEdges() const
{
	return this->edges;
}

halfEdge* Mesh_DCEL::getHalfEdge(unsigned int id)
{
	halfEdge* e = &(this->edges[id]);
	return e;
}

const halfEdge* Mesh_DCEL::getHalfEdge(unsigned int id) const
{
	const halfEdge* e = &(this->edges[id]);
	return e;
}

halfEdge* Mesh_DCEL::getHalfEdge(Vertex* vertexA, Vertex* vertexB)
{
	EdgeIterator it(vertexA);
	while (it.hasNext())
	{
		halfEdge* e = it.getNext();
		if (e->get_twinEdge()->get_startVertex() == vertexB)
		{
			return e;
		}
	}
	return NULL;
}

halfEdge* Mesh_DCEL::getHalfEdge(unsigned int vertexIdA, unsigned int vertexIdB)
{
	return getHalfEdge(getVertex(vertexIdA), getVertex(vertexIdB));
}

unsigned int Mesh_DCEL::getHalfEdgeId(halfEdge* HalfEdge) const
{
	const halfEdge* firstEdge = &(this->edges[0]);
	unsigned int id = HalfEdge - firstEdge;
	if (id >= 0 && id < this->edges.size())
	{
		if (&(this->edges[id]) == HalfEdge)
			return id;
	}
	return MESH_NULL_ID;
}

unsigned int Mesh_DCEL::getNumHalfEdges() const
{
	return this->edges.size();
}

halfEdge* Mesh_DCEL::findIncidentalHalfEdge(Vertex* vertex)
{

	halfEdge* edgeTmp = NULL;
	halfEdge* result = NULL;
	int nullFacesCount = 0;
	EdgeIterator it(vertex);
	while (it.hasNext())
	{
		edgeTmp = it.getNext();
		if (edgeTmp->get_twinEdge()->get_Face() == NULL)
		{
			result = edgeTmp->get_twinEdge();
			nullFacesCount++;
		}
	}
	if (nullFacesCount != 1)
	{
		result = NULL;
	}
	return result;
}

std::vector<Triangle>& Mesh_DCEL::getMesh()
{
	return this->getFaces();
}

const std::vector<Triangle>& Mesh_DCEL::getMesh() const
{
	return this->getFaces();
}

void Mesh_DCEL::setBottomLeftVertex(const Point3D& p)
{
	bottomLeftVertex = p;
}

void Mesh_DCEL::setUpperRightVertex(const Point3D& p)
{
	upperRightVertex = p;
}

Point3D Mesh_DCEL::getBottomLeftVertex() const
{
	return bottomLeftVertex;
}

Point3D Mesh_DCEL::getUpperRightVertex() const
{
	return upperRightVertex;
}

void Mesh_DCEL::setUpperBottomVertices(Point3D val)
{
	if (val.get_x() < bottomLeftVertex.get_x()) bottomLeftVertex.set_x(val.get_x());

	if (val.get_y() < bottomLeftVertex.get_y()) bottomLeftVertex.set_y(val.get_y());

	if (val.get_z() < bottomLeftVertex.get_z()) bottomLeftVertex.set_z(val.get_z());

	if (val.get_x() > upperRightVertex.get_x()) upperRightVertex.set_x(val.get_x());

	if (val.get_y() > upperRightVertex.get_y()) upperRightVertex.set_y(val.get_y());

	if (val.get_z() > upperRightVertex.get_z()) upperRightVertex.set_z(val.get_z());

}

void Mesh_DCEL::clear() {
	this->vertices.clear();
	this->edges.clear();
	this->faces.clear();
	this->unhandledTriangles.clear();
	this->unhandledTrianglesCount = 0;
}