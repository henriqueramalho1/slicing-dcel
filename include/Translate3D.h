/* ---------------------------------------------------------------------------------------------------------------------------------------- \
	Classe: Translate3D
	Descri��o: Classe respons�vel pela transla��o.
	Cria��o: ??/??/????									�ltima Modifica��o: ??/??/????
\ ---------------------------------------------------------------------------------------------------------------------------------------- */

#pragma once

#include "Transform3D.h"

/* ---------------------------------------------------------------------------------------------------------------------------------------- \
	Classe: Translate3D
	Descri��o: Classe respons�vel pela transla��o.
\ ---------------------------------------------------------------------------------------------------------------------------------------- */
class Translate3D :
	public Transform3D
{
public:
	Translate3D(void);
	Translate3D(Point3D transform);
	Translate3D(float parX, float parY, float parZ);
	~Translate3D(void);

	virtual Point3D* pointTransform(Point3D* point, float parX, float parY, float parZ);
	virtual Vertex* vertexTransform(Vertex* vertex, float parX, float parY, float parZ);
};

