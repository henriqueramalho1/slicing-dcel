/* ---------------------------------------------------------------------------------------------------------------------------------------- \
	Classe: Transform3D
	Descri��o: Classe abstrata para realizar alguma transforma��o.
	Cria��o: ??/??/????									�ltima Modifica��o: ??/??/????
\ ---------------------------------------------------------------------------------------------------------------------------------------- */

#include "Transform3D.h"

/* ------------------------------------------------------------------------------------------------- \
	M�todo: Construtra da Classe
	Descri��o: Cria um novo objeto do tipo Transform3D.
\ ------------------------------------------------------------------------------------------------- */
Transform3D::Transform3D(void):parX(0.0),parY(0.0),parZ(0.0){}

/* ------------------------------------------------------------------------------------------------- \
	M�todo: Construtra da Classe
	Descri��o: Cria um novo objeto do tipo Transform3D.
\ ------------------------------------------------------------------------------------------------- */
Transform3D::Transform3D(Point3D transform):parX(transform.get_x()),parY(transform.get_y()), parZ(transform.get_z()){}

/* ------------------------------------------------------------------------------------------------- \
	M�todo: Construtra da Classe
	Descri��o: Cria um novo objeto do tipo Transform3D.
\ ------------------------------------------------------------------------------------------------- */
Transform3D::Transform3D(float parX, float parY, float parZ):parX(parX),parY(parY), parZ(parZ){}

/* ------------------------------------------------------------------------------------------------- \
	M�todo: Destrutora da Classe
	Descri��o: Destr�i o objeto do tipo Transform3D.
\ ------------------------------------------------------------------------------------------------- */
Transform3D::~Transform3D(void){}


/* ------------------------------------------------------------------------------------------------- \
	M�todo: setTransform
	Descri��o: .
\ ------------------------------------------------------------------------------------------------- */
void Transform3D::setTransform(float parX, float parY, float parZ)
{
	this->parX = parX;
	this->parY = parY;
	this->parZ = parZ;
}

/* ------------------------------------------------------------------------------------------------- \
	M�todo: setTransform
	Descri��o: .
\ ------------------------------------------------------------------------------------------------- */
void Transform3D::setTransform(Point3D transform)
{
	setTransform(transform.get_x(),transform.get_y(),transform.get_y());
}


/* ------------------------------------------------------------------------------------------------- \
	M�todo: meshTransform
	Descri��o: .
\ ------------------------------------------------------------------------------------------------- */
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

	return mesh;
}

/* ------------------------------------------------------------------------------------------------- \
	M�todo: meshTransform
	Descri��o: .
\ ------------------------------------------------------------------------------------------------- */
Mesh_DCEL* Transform3D::meshTransform(Mesh_DCEL* mesh)
{
	return meshTransform(mesh, parX, parY, parZ);
}


/* ------------------------------------------------------------------------------------------------- \
	M�todo: triangleTransform
	Descri��o: .
\ ------------------------------------------------------------------------------------------------- */
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

/* ------------------------------------------------------------------------------------------------- \
	M�todo: triangleTransform
	Descri��o: .
\ ------------------------------------------------------------------------------------------------- */
Triangle* Transform3D::triangleTransform(Triangle* triangle)
{
	return triangleTransform(triangle, parX, parY, parZ);
}


/* ------------------------------------------------------------------------------------------------- \
	M�todo: pointTransform
	Descri��o: .
\ ------------------------------------------------------------------------------------------------- */
Point3D* Transform3D::pointTransform(Point3D* point)
{
	return pointTransform(point, parX, parY, parZ);
}

Vertex* Transform3D::vertexTransform(Vertex* vertex)
{
	return vertexTransform(vertex, parX, parY, parZ);
}

