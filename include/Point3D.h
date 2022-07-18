/* ---------------------------------------------------------------------------------------------------------------------------------------- \
	Classe: Point3D
	Descrição: Classe que representa um ponto no espaço cartesiano, com funções de acesso às variáveis.
	Criação: ??/??/????									Última Modificação: 27/08/2014

	Autor: Luiz Vasconcellos							Comentado inicialmente por: Guilherme Jacichen e Luiz Vasconcellos (Data: 25/08/15)
\ ---------------------------------------------------------------------------------------------------------------------------------------- */

#pragma once
#include<functional>
#include "HalfEdge.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/vector_angle.hpp"

class Point3D
{
public:
	Point3D(const float x  = 0.0f, const float y = 0.0f, const float z = 0.0f);
	
	Point3D(const Point3D &point);

	~Point3D(void);

	void transform (const glm::mat4 &mat) { 
      glm::vec4 v = glm::vec4(fX, fY, fZ, 1.0);
      glm::vec4 vt = mat*v; 
      fX = (vt.x); fY = (vt.y); fZ = (vt.z);
    }

	float get_x() const;

	float get_y() const;
	
	float get_z() const;

	Point3D* get_point () const;

	void set_x (const float x);

	void set_y (const float y);

	void set_z (const float z);

	void set_point(const Point3D &point);

	bool is_z_bigger (const Point3D &point) const;

	bool is_z_smaller (const Point3D &point) const;

	float distance (Point3D p);

	Point3D operator+ (const Point3D &point) const;
	
	Point3D operator- (const Point3D &point) const;
	
	Point3D operator- () const;

	Point3D operator* (const float &num) const;

	Point3D operator/ (const float &num) const;

	virtual void operator= (const Point3D &point);

	virtual bool operator== (const Point3D &point) const;

	virtual bool operator!= (const Point3D &point) const;

	virtual bool operator> (const Point3D &point) const;

	virtual bool operator< (const Point3D &point) const;
	
protected:
	float fX;/*!< x coordinate*/
	float fY;/*!< y coordinate*/
	float fZ;/*!< z coordinate*/
};
	

namespace std {

/*!
 * Specialisation of std::hash to hash Point3.
 */
template<> 
struct hash<Point3D> {
	/*!
	 * Calls the hash struct, which returns the hash.
	 */
	std::size_t operator ()(const Point3D& point) const;
};


}
