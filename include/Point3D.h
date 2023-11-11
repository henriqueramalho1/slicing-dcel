#pragma once
#include <functional>
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
	~Point3D();

	void transform (const glm::mat4 &mat) { 
      glm::vec4 v = glm::vec4(fX, fY, fZ, 1.0);
      glm::vec4 vt = mat*v; 
      fX = (vt.x); fY = (vt.y); fZ = (vt.z);
    }

	float get_x() const;
	float get_y() const;
	float get_z() const;
	void set_x (const float x);
	void set_y (const float y);
	void set_z (const float z);
	void set_point(const Point3D &point);
	
private:

	float fX;
	float fY;
	float fZ;
};