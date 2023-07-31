#include "Mesh_DCEL.h"

Mesh_DCEL::Mesh_DCEL():
upperRightVertex(-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max(), -std::numeric_limits<float>::max()),
bottomLeftVertex(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max())
{

}

Mesh_DCEL::~Mesh_DCEL()
{
	this->clear();
}

void Mesh_DCEL::roundVertices()
{
	double eps = 0.004;

	for(size_t i = 0; i < vertices.size(); i++)
	{
		vertices[i].getPoint().set_x(xround(vertices[i].getPoint().get_x(), eps, 2, 0));
		vertices[i].getPoint().set_y(xround(vertices[i].getPoint().get_y(), eps, 2, 0));
		vertices[i].getPoint().set_z(xround(vertices[i].getPoint().get_z(), eps, 2, 0));
	}
}

float Mesh_DCEL::xround (float x, double eps, int mod, int rem) {
  	double y = round((double)x/(mod * eps));
  	double z = (y * mod + rem) * eps;
  	return (float)z;
}

std::vector<Triangle>& Mesh_DCEL::getFaces()
{
	return this->faces;
}
	
const std::vector<Triangle>& Mesh_DCEL::getFaces() const
{
	return this->faces;
}


std::vector<std::vector<int>>& Mesh_DCEL::getFaceIndices()
{
	return this->face_indices;
}

const std::vector<std::vector<int>>& Mesh_DCEL::getFaceIndices() const
{
	return this->face_indices;
}

const std::vector<Vertex>& Mesh_DCEL::getVertices() const
{
	return this->vertices;
}

std::vector<Vertex>& Mesh_DCEL::getVertices()
{
	return this->vertices;
}

std::vector<halfEdge>& Mesh_DCEL::getHalfEdges()
{
	return this->edges;
}

const std::vector<halfEdge>& Mesh_DCEL::getHalfEdges() const
{
	return this->edges;
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

void Mesh_DCEL::updateTriangles()
{
	for(Triangle& t : faces)
	{
		t.set_z_min_max();
	}
}

void Mesh_DCEL::clear() {
	this->vertices.clear();
	this->edges.clear();
	this->faces.clear();
}

Point3D Mesh_DCEL::AABBSize()
{
	 return Point3D ( upperRightVertex.get_x() - bottomLeftVertex.get_x(), 
                    upperRightVertex.get_y() - bottomLeftVertex.get_y(), 
                    upperRightVertex.get_z() - bottomLeftVertex.get_z() );
}