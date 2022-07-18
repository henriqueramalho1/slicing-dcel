/** ---------------------------------------------------------------------------------------------------------------------------------------- \
	Descrição: Contém funções matemáticas gerais.
	Criação: ??/??/????									Última Modificação: 26/08/2014

	Port: Leandro
\ ---------------------------------------------------------------------------------------------------------------------------------------- */

#include "math_functions.h"

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#define COPYSIGN _copysign
#else
#define COPYSIGN std::copysign
#endif

/** ------------------------------------------------------------------------------------------------- \
	Método: roundIt
	Descrição: Arredondamento.
\ ------------------------------------------------------------------------------------------------- */
float roundIt (const float &num, const int &prec)
{
	double _num = num;

	return (double) ((int) (_num* prec + 0.5))/prec;
}


/** ------------------------------------------------------------------------------------------------- \
	Método: compf
	Descrição: Compara dois pontos flutuantes utilizando um epsilon como base.
\ ------------------------------------------------------------------------------------------------- */
bool compf (const float &a, const float &b, float epsilon )
{
	float diff = abs (a - b);
	if( diff < epsilon)
		return true;
	return false;

	//TRUE = igual, FALSE = diferente
}

/** ------------------------------------------------------------------------------------------------- \
	Método: fullcompf
	Descrição: Compara dois pontos flutuantes utilizando um epsilon como base. Retorna 1 se a > b, -1 se a < b e 0 se a == b.
\ ------------------------------------------------------------------------------------------------- */
COMPF_RESULT fullcompf(const float& a, const float& b, float epsilon)
{
	float diff = a - b;
	if (diff < -epsilon)
		return COMPF_SMALLER;
	if (diff > epsilon)
		return COMPF_BIGGER;
	return COMPF_EQUAL;
}


/** ------------------------------------------------------------------------------------------------- \
	Método: degreeToRadian
	Descrição: Converte graus para radianos.
\ ------------------------------------------------------------------------------------------------- */
float degreeToRadian (float degree)
{
	return degree * (2*PI/360);
}

/** ------------------------------------------------------------------------------------------------- \
	Método: radianToDegreee
	Descrição: Converte radianos para graus.
\ ------------------------------------------------------------------------------------------------- */
float radianToDegreee (float radian)
{
	return radian * ( 360/ (2*PI));
}

/** ------------------------------------------------------------------------------------------------- \

	Descrição: Calcula o s da equação parametrica da interseção entre duas retas
\ ------------------------------------------------------------------------------------------------- */

float parametricIntersectionS(Point3D* point_a, Point3D* point_b, Point3D* point_c, Point3D* point_d)
{
	float s= ( ( (point_d->get_x()-point_c->get_x() )*(point_c->get_y()-point_a->get_y() )-
		( (point_d->get_y()-point_c->get_y() )*(point_c->get_x()-point_a->get_x() ) ) )/
		( (point_d->get_x()-point_c->get_x() )*(point_b->get_y()-point_b->get_y() )-
		( (point_d->get_y()-point_c->get_y() )*(point_b->get_x()-point_b->get_x() ) ) ) );
	return s;
}


/** ------------------------------------------------------------------------------------------------- \

	Descrição: Calcula o t da equação parametrica da interseção entre duas retas
\ ------------------------------------------------------------------------------------------------- */

float parametricIntersectionT(Point3D* point_a, Point3D* point_b, Point3D* point_c, Point3D* point_d)
{
	float t1 =   ( (point_b->get_x()-point_a->get_x() )*(point_c->get_y()-point_a->get_y() )-
		( (point_b->get_y()-point_a->get_y() )*(point_c->get_x()-point_a->get_x() ) ) );

	float t2 =  (point_d->get_x()-point_c->get_x() )*(point_b->get_y()-point_a->get_y() )-
		( (point_d->get_y()-point_c->get_y() )*(point_b->get_x()-point_a->get_x() ) )  ;
	t1 = t1/t2;
	return t1;
}


/**--------------------------------------------------------------------------------------------------
método para calculo da area sinalizada de um triangulo
--------------------------------------------------------------------------------------------------*/
float triangleArea (Point3D* point_a, Point3D* point_b, Point3D* point_c)
{
	float area = point_a->get_x() * point_b->get_y() - point_b->get_x() * point_a->get_y()+
		point_c->get_x() * point_a->get_y()-point_a->get_x() * point_c->get_y() +
		point_b->get_x() * point_c->get_y()-point_c->get_x() * point_b->get_y();
	return (area);
}

/**--------------------------------------------------------------------------------------------------
método para identificar posição relativa entre um ponto e uma reta
 1- esquerda
 0- contido
-1 - direita 
--------------------------------------------------------------------------------------------------*/
int pointPosition (Point3D* point_a, Point3D* point_b, Point3D* point_c)
{
	float area = triangleArea (point_a, point_b, point_c);
	if (area >0)
		return 1;
	if (area<0)
		return -1;
	return 0;
}


/**--------------------------------------------------------------------------------------------------
método PIP direcional

 1 - dentro
-1 - fora
--------------------------------------------------------------------------------------------------*/
int directionalPIP (Point3D* point_a, Point3D* point_b, PointList* _list)
{
	int n_intersections = 0;
	for (int i = 0; i < _list->getSize();i++)
	{
		if (!(_list->getPoint(i)->get_x() == _list->getPoint((i+1)% _list->getSize())->get_x() &&
	  _list->getPoint(i)->get_y() == _list->getPoint((i+1)% _list->getSize())->get_y()))
		{
			int s1 = pointPosition(point_a, point_b, _list->getPoint(i));
			int s2 = pointPosition(point_a, point_b, _list->getPoint((i+1)% _list->getSize())); 
			int s3 = pointPosition(point_a, _list->getPoint(i), _list->getPoint((i+1)% _list->getSize()));
			int s4 = pointPosition(point_b, _list->getPoint(i), _list->getPoint((i+1)% _list->getSize()));
			if ((s1 *s2<0 ||(s1 +s2 == 1 && s1*s2 ==0))&& s3*s4 <0)
				n_intersections ++;
		}
	}
		if(n_intersections%2)
			return 1;
		return -1;
}

/** ------------------------------------------------------------------------------------------------- \
	Método: pointInPolygon
	Descrição: Define se um ponto está dentro ou fora de um PO-LI-GO-NO. 
	Funcionamento: ler artigo do Julian. Pedir para o Neri.
	Valores válidos:
		-1 - Para fora.
		0  - Para pertecendo à linha poligonal.
		1  - Para dentro.

	Erro detectado 26/07/2021: em algumas camadas os pontos do mesmo contorno retornam resultados diferentes utlizando Incremental Slicer.
\ ------------------------------------------------------------------------------------------------- */
int pointInPolygon(Point3D* point ,PointList* _list)
{
	int t = -1;
	for (int i = 0 ; i < _list->getSize() - 1; i++)
	{
		t *= pointHelp(*point,*_list->getPoint(i),*_list->getPoint(i+1));
	}

	// Fechar o contorno.
	return ( t * pointHelp(*point,
		*_list->getPoint(_list->getSize() - 1),*_list->getPoint(0)) ) ;
}

/** ------------------------------------------------------------------------------------------------- \
	Método: pointHelp
	Descrição: Função auxiliar do pointInPolygon.
\ ------------------------------------------------------------------------------------------------- */
int pointHelp(Point3D point, Point3D p1, Point3D p2)
{
	if ( compf(point.get_y(), p1.get_y()) &&
		 compf(p1.get_y() , p2.get_y()) )
	{
		if( ( p1.get_x() <= point.get_x() && point.get_x() <= p2.get_x() ) &&
			( p2.get_x() <= point.get_x() && point.get_x() <= p1.get_x() ) )
			return 0;
		else return 1;
	}

	if (p1.get_y() > p2.get_y())
	{
		Point3D swap = p1;
		p1 = p2;
		p2 = swap;
	}

	if ( point.get_y() < p1.get_y() || point.get_y() > p2.get_y())
		return 1;

	float det = ( ( (p1.get_x() - point.get_x() ) * (p2.get_y() - point.get_y()) ) 
		-  ( (p1.get_y() - point.get_y()) * (p2.get_x() - point.get_x()) ) );

	if (det > 0)
	{
		if ( compf(p1.get_y(),point.get_y()))
			return 1;
		else return -1;
	}
	else if (det < 0)
		return 1;
	else return 0;
}

/** ------------------------------------------------------------------------------------------------- \
	Método: pointsDistanceAxisX
	Descrição: Calcula a distância entre dois pontos com relação ao eixo X.
\ ------------------------------------------------------------------------------------------------- */
float pointsDistanceAxisX (Point3D* p1, Point3D* p2)
{
	float distance = p2->get_x() - p1->get_x();
	return distance;
}

/** ------------------------------------------------------------------------------------------------- \
	Método: pointsDistanceAxisY
	Descrição: Calcula a distância entre dois pontos com relação ao eixo Y.
\ ------------------------------------------------------------------------------------------------- */
float pointsDistanceAxisY (Point3D* p1, Point3D* p2)
{
	float distance = p2->get_y() - p1->get_y();
	return distance;
}


void swap(float* a, float* b)  
{  
    int t = *a;  
    *a = *b;  
    *b = t;  
}  

/** ------------------------------------------------------------------------------------------------- \
	Método: pointLineDistance
	Descrição: Calcula o módulo de um segmento de linha (?).
\ ------------------------------------------------------------------------------------------------- */
float pointLineDistance (Point3D p1, Point3D p2/*, Point3D bet //este parametro existia no codigo antigo, mas era ignorado*/)
{
	float distance = (p1.get_x() - p2.get_x())* (p1.get_x() - p2.get_x()) + (p1.get_y() - p2.get_y()) * (p1.get_y() - p2.get_y());
	distance = sqrt(distance);
	return distance;
}

/** ------------------------------------------------------------------------------------------------- \
	Método: pointAngleBetween
	Descrição: Cálcula o ângulo de um triângulo P1,^Bet,P2.
\ ------------------------------------------------------------------------------------------------- */
float pointAngleBetween (Point3D p1, Point3D p2, Point3D bet)
{
	Point3D v21 = p1 - bet;
	Point3D v23 = p2 - bet;

	float angle = angleBetweenVectors(v21,v23);

	return angle;
}


/** ------------------------------------------------------------------------------------------------- \
	Método: deletePointAngle
	Descrição: Deleta todos os pontos de uma lista que sejam "colineares" (angulo < angle).
\ ------------------------------------------------------------------------------------------------- */
void deletePointAngle (PointList* _list, float angle)
{
	if(_list == nullptr)
		return;

	for(int i = 0; i < _list->getSize(); i++)
	{
		if( _list->getSize() < 4)
			break;

		if (PI-abs(pointAngleBetween(*_list->getPointPrev(i),*_list->getPointNext(i),*_list->getPoint(i))) < angle)
			_list->removePoint(i--);
	}
}



/** ------------------------------------------------------------------------------------------------- \
	Método: getNormal
	Descrição: Retorna o ângulo da reta normal ao segmento P1P2.
\ ------------------------------------------------------------------------------------------------- */
float getNormal (bool flag, Point3D p1, Point3D p2)
{
	float x1 = p1.get_x();
	float x2 = p2.get_x();
	float y1 = p1.get_y();
	float y2 = p2.get_y();

	float angle = 0;
	float normal = 0;

	float dx = x2 - x1;
	float dy = y2 - y1;

	angle = (float) atan2(dy,dx);

	if (angle < 0)
		angle += 2 * PI;
	else if (angle > 2 * PI)
		angle -= 2 * PI;

	if(flag)
		normal = angle + (float)PI/2/* - (float) PI/4*/;
	else
		normal = angle - (float)PI/2 /*- (float) PI/4*/;
	return normal;
}

/** ------------------------------------------------------------------------------------------------- \
	Método: getSentido
	Descrição: Retorna o sentido.
\ ------------------------------------------------------------------------------------------------- */
bool getSentido (Point3D p1, Point3D p2)
{
	//True para sentido horario, false para sentido antihorario
	return externalProduct(p1,p2).get_z() < 0;
}

/** ------------------------------------------------------------------------------------------------- \
	Método: dotProduct
	Descrição: Produto interno dos pontos.
\ ------------------------------------------------------------------------------------------------- */
float dotProduct (Point3D p1, Point3D p2)
{
	return p1.get_x()*p2.get_x() + p1.get_y()*p2.get_y() + p1.get_z()*p2.get_z();
}

/** ------------------------------------------------------------------------------------------------- \
	Método: externalProduct
	Descrição: Produto externo dos pontos.
\ ------------------------------------------------------------------------------------------------- */
Point3D externalProduct (Point3D p1, Point3D p2)
{
	float x = p1.get_y() * p2.get_z() - p2.get_y()*p1.get_z();
	float y = p2.get_x() * p1.get_z() - p1.get_x()*p2.get_z();
	float z = p1.get_x() * p2.get_y() - p2.get_x()*p1.get_y();

	return Point3D(x,y,z);
}

/** ------------------------------------------------------------------------------------------------- \
	Método: angleBetweenVectors
	Descrição: Ângulo entre os vetores.
\ ------------------------------------------------------------------------------------------------- */
float angleBetweenVectors (Point3D p1, Point3D p2)
{
	float internalProduct = dotProduct(p1,p2);
	float mod1 = sqrt(p1.get_x()*p1.get_x() + p1.get_y()*p1.get_y() + p1.get_z()* p1.get_z() );
	float mod2 = sqrt(p2.get_x()*p2.get_x() + p2.get_y()*p2.get_y() + p2.get_z()* p2.get_z() );
	float modProduct = mod1 * mod2;

	float cosine = roundIt(internalProduct/modProduct,100000);

	return roundIt(acos(cosine),100000);
}


/** ------------------------------------------------------------------------------------------------- \
	Método: getIntersecPoint
	Descrição: Retorna o (possível) ponto de intersecção entre dois segmentos (P1P2 e P3P4).
\ ------------------------------------------------------------------------------------------------- */
bool getIntersecPoint (Point3D* p1, Point3D* p2, Point3D* p3, Point3D* p4, Point3D* pi, int roundFactor)
{
	/*
	getIntersecPoint -> para segmentos de reta
	#1

	Verifica qual o ponto de interseção das retas P1-P2 e P3-P4, armazena
	esse ponto em pi por referência e verifica se o ponto encontrado é
	um dos pontos passados por parâmetro.
	>p1: junto com p2 definem a reta P1-P2;
	>p2: junto com p1 definem a reta P1-P2;
	>p3: junto com p4 definem a reta P3-P4;
	>p4: junto com p3 definem a reta P3-P4;
	>pi: ponto de interesecção (valor retornado por referência);
	>return: false caso o ponto de interssecção seja um dos pontos passados 
	parâmetro, true caso contrário.
	*/

	//Coordenadas dos extremos dos segmentos
	float x1 = p1->get_x(); float y1 = p1->get_y(); 
	float x2 = p2->get_x(); float y2 = p2->get_y();
	float x3 = p3->get_x(); float y3 = p3->get_y();
	float x4 = p4->get_x(); float y4 = p4->get_y();
	float z = p1->get_z();
	//Coordenadas da intersecção
	float xi = 0; float yi = 0; float zi = z;

	if(compf(x1,7.23f) && compf(y1,14.7f) && compf(x2,7.08f) && compf(y2,14.9f) && compf(x3,7.02f) && compf(y3,14.54f) && compf(x4,7.23f) && compf(y4,14.71f))
		int i = 0;

	//Coordendas dos vetores direção
	float xd1 = x2 - x1; float xd2 = x4 - x3;
	float yd1 = y2 - y1; float yd2 = y4 - y3;

	//se os dois segmentos forem verticais ou horizontais 
	if( ( compf(y1,y2) && compf(y3,y4)) || (compf(x1,x2) && compf(x3,x4)))
		return false;

	//se os dois segmentos forem formados a partir do memso vetor direção
	if ( compf(xd1,xd2) && compf(yd1,yd2) )
		return false;

	float det  = (x1-x2) * (y3 - y4) - (y1-y2) * (x3 - x4);
	det = abs(det);

	//se det for 0 não há intersecção
	if(compf(det,0))
		return false;

	float  s = ( (y1-y2) * (x3 - x1) - (x1 - x2) * (y3 - y1)) / ( (x1-x2) *(y3-y4) - (y1-y2) * (x3-x4));

	xi = x3 + (x3-x4) * s;
	yi = y3 + (y3-y4) * s;
	
	/*
	xi = correctCoord(x1,xi);
	xi = correctCoord(x2,xi);
	xi = correctCoord(x3,xi);
	xi = correctCoord(x4,xi);

	yi = correctCoord(y1,yi);
	yi = correctCoord(y2,yi);
	yi = correctCoord(y3,yi);
	yi = correctCoord(y4,yi);
	*/

	xi = (float) roundIt(xi,roundFactor);
	yi = (float) roundIt(yi,roundFactor);

	pi->set_point(Point3D(xi,yi,z));

	//Condições para que  a intersecção seja entre os pontos determinados do segmento
	if ((fullcompf(yi, y1) == COMPF_SMALLER && fullcompf(yi, y2) == COMPF_SMALLER) ||
		(fullcompf(yi, y1) == COMPF_BIGGER  && fullcompf(yi, y2) == COMPF_BIGGER))
		return false;
	if ((fullcompf(yi, y3) == COMPF_SMALLER && fullcompf(yi, y4) == COMPF_SMALLER) ||
		(fullcompf(yi, y3) == COMPF_BIGGER  && fullcompf(yi, y4) == COMPF_BIGGER))
		return false;
	if ((fullcompf(xi, x1) == COMPF_SMALLER && fullcompf(xi, x2) == COMPF_SMALLER) ||
		(fullcompf(xi, x1) == COMPF_BIGGER  && fullcompf(xi, x2) == COMPF_BIGGER))
		return false;
	if ((fullcompf(xi, x3) == COMPF_SMALLER && fullcompf(xi, x4) == COMPF_SMALLER) ||
		(fullcompf(xi, x3) == COMPF_BIGGER  && fullcompf(xi, x4) == COMPF_BIGGER))
		return false;

	return true;
}

/** ------------------------------------------------------------------------------------------------- \
	Método: getRectIntersecPoint
	Descrição: Retorna o (possível) ponto de intersecção entre duas retas (P1P2 e P3P4).
\ ------------------------------------------------------------------------------------------------- */
bool getRectIntersecPoint (Point3D* p1, Point3D* p2, Point3D* p3, Point3D* p4, Point3D* pi)
{
	//getRectIntercPoint -> para retas infinitas

	//Coordenadas dos extremos dos segmentos
	float x1 = p1->get_x(); float y1 = p1->get_y(); 
	float x2 = p2->get_x(); float y2 = p2->get_y();
	float x3 = p3->get_x(); float y3 = p3->get_y();
	float x4 = p4->get_x(); float y4 = p4->get_y();
	float z = p1->get_z();
	//Coordenadas da intersecção
	float xi = 0; float yi = 0; float zi = z;

	//Coordendas dos vetores direção
	float xd1 = x2 - x1; float xd2 = x4 - x3;
	float yd1 = y2 - y1; float yd2 = y4 - y3;

	//se os dois segmentos forem formados a partir do memso vetor direção
	if ( compf(xd1,xd2) && compf(yd1,yd2) )
		return false;

	//se os dois segmentos forem verticais ou horizontais 
	if( ( compf(y1,y2) && compf(y3,y4)) || (compf(x1,x2) && compf(x3,x4)))
		return false;

	float det  = (x1-x2) * (y3 - y4) - (y1-y2) * (x3 - x4);
	det = abs(det);

	//se det for 0 não há intersecção
	if(compf(det,0))
		return false;

	float  s = ( (y1-y2) * (x3 - x1) - (x1 - x2) * (y3 - y1)) / ( (x1-x2) *(y3-y4) - (y1-y2) * (x3-x4));

	xi = x3 + (x3-x4) * s;
	yi = y3 + (y3-y4) * s;
	
	xi = correctCoord(x1,xi);
	xi = correctCoord(x2,xi);
	xi = correctCoord(x3,xi);
	xi = correctCoord(x4,xi);

	yi = correctCoord(y1,yi);
	yi = correctCoord(y2,yi);
	yi = correctCoord(y3,yi);
	yi = correctCoord(y4,yi);

	xi = (float) roundIt(xi);
	yi = (float) roundIt(yi);

	pi->set_point(Point3D(xi,yi,z));

	return true;
}

/** ------------------------------------------------------------------------------------------------- \
	Método: correctCoord
	Descrição: Correção  auxiliar para o método getIntersectPoint.
\ ------------------------------------------------------------------------------------------------- */
float correctCoord(float coord1, float coord2)
{
	if ( abs(coord1 - coord2) <= 0.001)
		coord2 = coord1;

	return coord2;
}

//Codigo Antigo: da onde surge essa área?
//float calcArea (SolidContour* contour)
//{
//	//sanity check
//	if(contour == nullptr)
//		throw("Contour pointer is a null pointer");
//	if(contour->getSize() < 3)
//		throw("Contour doesn't have an area");
//
//	double x1;
//	double x2;
//	double y1;
//	double y2;
//
//	float area = 0;
//	int index = 0;
//	contour->setPointIndex(index);
//
//	//Primeiros pontos
//	Point3D* p1 = contour->getCurrentPoint();
//	contour->advancePoint();
//	Point3D* p2 = contour->getCurrentPoint();
//	index = contour->getCurrentPointIndex();
//	//Varredura do contorno
//	do
//	{
//		x1 = p1->get_x();
//		x2 = p2->get_x();
//		y1 = p1->get_y();
//		y2 = p2->get_y();
//
//		area += (x2 - x1) * (y1 + y2) / 2.0f;
//
//		p1 = p2;
//		index = contour->getAdvancePointIndex();
//		p2 = contour->getCurrentPoint();
//		
//	}while(index != contour->getSize());
//
//	return area;
//}

/** ------------------------------------------------------------------------------------------------- \
	Método: calcArea
	Descrição: Calcula a área de um contorno/offset.
\ ------------------------------------------------------------------------------------------------- */
float calcArea (SolidContour* contour)
{
 /**
	A área de um contorno, positivo se o sentido for horário,
	negativo cc. Contornos com laço retornam a diferença
	das áreas limitadas pelos laços simples 
	Considera-se que todos os pontos tenham a mesma cota (z)
	
	Descrição do algoritmo: somar áreas dos triângulos do polígono, 
	utilizando para isso o módulo do produto externo de dois vetores:
	
							[ i^  j^   k^ ]
	Area = k^Mag(UxV)/2 =	[ Ux  Uy   0  ]*1/2
							[ Vx  Vy   0  ]
	
	Onde A, B e C são pontos que indicam os triângulos do polígono.
	U e V são vetores distância dos pontos AB e AC, respectivamente.
	O produto externo da a área do paralelogramo (em k^, já que os 
	pontos estão no mesmo plano), e esta orientado segundo a regra
	da mão direita (positivo - CCW/ negativo - CW). O triângulo possui 
	área igual à metade da magnitude em k^.
*/

	//sanity check
	if(contour == nullptr)
		throw("Contour pointer is a null pointer");
	if(contour->getSize() < 3)
		throw("Contour doesn't have an area");

	double ux;
	double uy;
	double vx;
	double vy;

	float area = 0;
	contour->setPointIndex(0);

	//Primeiros pontos
	//Ponto de origem A
	Point3D* a = contour->getCurrentPoint();
	contour->advancePoint();
	//Ponto iterador B
	Point3D* b = contour->getCurrentPoint();
	contour->advancePoint();
	//Ponto iterador C
	Point3D* c = contour->getCurrentPoint();

	//Varredura do contorno
	do
	{
		//Componentes do vetor distância

		ux = a->get_x() - b->get_x();
		uy = a->get_y() - b->get_y();
		vx = a->get_x() - c->get_x();
		vy = a->get_y() - c->get_y();

		//Resultado da determinante
		area += ((ux*vy)-(uy*vx))/2;

		//Próximo passo da iteração
		b = c;
		contour->advancePoint();
		c = contour->getCurrentPoint();
		
	}while(!contour->isPointLast());

	return area;
}

/** ------------------------------------------------------------------------------------------------- \
	Método: normPointList
	Descrição: .
\ ------------------------------------------------------------------------------------------------- */
void normPointList(PointList* _list, int prec)
{
	Point3D* point;
	float x, y, z;

	for(int i = 0; i < _list->getSize() ; i++)
	{
		point = _list->getPoint(i);
		x = point->get_x();
		y = point->get_y();
		z = point->get_z();

		x = roundIt(x,prec);
		y = roundIt(y,prec);
		z = roundIt(z,prec);

		point->set_point(Point3D(x,y,z));
	}
}

/** ------------------------------------------------------------------------------------------------- \
	Método: Calcula o coeficiente linear
	Descrição: Calcula o coeficiente linear.
\ ------------------------------------------------------------------------------------------------- */
float linearCoef(const Point3D *p, const float angle)
{
	float x = p->get_x();
	float y = p->get_y();
		
	float t = (float)tan(angle);
	
	return y-(t*x);
}

/** ------------------------------------------------------------------------------------------------- \
	Método: arredNum
	Descrição: .
\ ------------------------------------------------------------------------------------------------- */
float arredNum(float Num, float Prec)
{
	int aux;
	if( Num >= 0 ) aux = 1;
	else aux = -1;


	double Pot = pow(10, Prec);

	Num = (float)fabs(Num);
	
	if((ceil(Pot * Num) - Pot * Num) <= 0.5)
	{
		Num = (float)(ceil(Pot * Num) / Pot);
	}

	if(Pot * Num - floor(Pot * Num) < 0.5)
	{
		Num = (float)(floor(Pot * Num) / Pot);
	}

	Num = aux * Num;
	
	return Num;
}


/** ------------------------------------------------------------------------------------------------- \
	Método: isConvex
	Descrição: Verifica se o ponto p2 eh convexo, sendo que P1, P2, e P3, nesta sequência, definem o sentido.
\ ------------------------------------------------------------------------------------------------- */
bool isConvex(Point3D* p1, Point3D* p2, Point3D* p3){
	Point3D vBegin = *p2 - *p1;
	Point3D vEnd = *p3 - *p2;

	return getSentido(vBegin,vEnd);
}
