/** ---------------------------------------------------------------------------------------------------------------------------------------- \
	Classe: Translate3D
	Descrição: Classe responsável pela translação.
	Criação: ??/??/????									Última Modificação: ??/??/????
\ ---------------------------------------------------------------------------------------------------------------------------------------- */

#include "Translate3D.h"

/** ------------------------------------------------------------------------------------------------- \
	Método: Construtora da Classe
	Descrição: Cria um novo objeto do tipo Transform3D.
\ ------------------------------------------------------------------------------------------------- */
Translate3D::Translate3D(void):Transform3D(){}

/** ------------------------------------------------------------------------------------------------- \
	Método: Construtora da Classe
	Descrição: Cria um novo objeto do tipo Transform3D.
\ ------------------------------------------------------------------------------------------------- */
Translate3D::Translate3D(Point3D transform):Transform3D(transform){}

/** ------------------------------------------------------------------------------------------------- \
	Método: Construtora da Classe
	Descrição: Cria um novo objeto do tipo Transform3D.
\ ------------------------------------------------------------------------------------------------- */
Translate3D::Translate3D(float parX, float parY, float parZ):Transform3D(parX, parY, parZ){}

/** ------------------------------------------------------------------------------------------------- \
	Método: Destrutora da Classe
	Descrição: Destrói o objeto do tipo Transform3D.
\ ------------------------------------------------------------------------------------------------- */
Translate3D::~Translate3D(void){}


/** ------------------------------------------------------------------------------------------------- \
	Método: pointTransform
	Descrição: .
\ ------------------------------------------------------------------------------------------------- */
Point3D* Translate3D::pointTransform(Point3D* point, float parX, float parY, float parZ)
{
	point->set_x(point->get_x()+parX);
	point->set_y(point->get_y()+parY);
	point->set_z(point->get_z()+parZ);
	return point;
}

Vertex* Translate3D::vertexTransform(Vertex* vertex, float parX, float parY, float parZ)
{
	vertex->getPoint().set_x(vertex->getPoint().get_x() + parX);
	vertex->getPoint().set_y(vertex->getPoint().get_y() + parY);
	vertex->getPoint().set_z(vertex->getPoint().get_z() + parZ);
	return vertex;
}
