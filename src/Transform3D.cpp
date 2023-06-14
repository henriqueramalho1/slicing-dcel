#include "Transform3D.h"

Transform3D::Transform3D(void):parX(0.0),parY(0.0),parZ(0.0)
{

}

Transform3D::Transform3D(Point3D transform):parX(transform.get_x()),parY(transform.get_y()), parZ(transform.get_z())
{

}

Transform3D::Transform3D(float parX, float parY, float parZ):parX(parX),parY(parY), parZ(parZ)
{

}

Transform3D::~Transform3D()
{

}

void Transform3D::setTransform(float parX, float parY, float parZ)
{
	this->parX = parX;
	this->parY = parY;
	this->parZ = parZ;
}

void Transform3D::setTransform(Point3D transform)
{
	setTransform(transform.get_x(),transform.get_y(),transform.get_y());
}

Mesh_DCEL* Transform3D::meshTransform(Mesh_DCEL* mesh, float parX, float parY, float parZ)
{
	mesh->setUpperRightVertex(Point3D(-999999, -999999, -999999));
	mesh->setBottomLeftVertex(Point3D(999999, 999999, 999999));

	
	std::vector<Vertex>& verticeslist = mesh->getVertices();
	for (auto it = verticeslist.begin(); it != verticeslist.end(); it++)
	{
		this->vertexTransform(&*it);
		mesh->setUpperBottomVertices(it->getPoint());
	}

	mesh->updateTriangles();

	return mesh;
}

Mesh_DCEL* Transform3D::meshTransform(Mesh_DCEL* mesh)
{
	return meshTransform(mesh, parX, parY, parZ);
}

Triangle* Transform3D::triangleTransform(Triangle* triangle, float, float, float)
{
	Point3D p1 = triangle->get_v1();
	Point3D p2 = triangle->get_v2();
	Point3D p3 = triangle->get_v3();

	triangle->set_v1(*pointTransform(&p1));
	triangle->set_v2(*pointTransform(&p2));
	triangle->set_v3(*pointTransform(&p3));
	return triangle;
}

Triangle* Transform3D::triangleTransform(Triangle* triangle)
{
	return triangleTransform(triangle, parX, parY, parZ);
}

Point3D* Transform3D::pointTransform(Point3D* point)
{
	return pointTransform(point, parX, parY, parZ);
}

Vertex* Transform3D::vertexTransform(Vertex* vertex)
{
	return vertexTransform(vertex, parX, parY, parZ);
}

