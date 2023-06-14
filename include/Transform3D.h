#pragma once

#include "Point3D.h"
#include "Vertex.h"
#include "Mesh_DCEL.h"

class Transform3D
{
public:
	Transform3D(void);
	Transform3D(Point3D transform);
	Transform3D(float parX, float parY, float parZ);
	~Transform3D(void);
	
	virtual void setTransform(float parX, float parY, float parZ);	
	virtual void setTransform(Point3D transform);

	virtual Mesh_DCEL* meshTransform(Mesh_DCEL* mesh, float parX, float parY, float parZ);
	virtual Mesh_DCEL* meshTransform(Mesh_DCEL* mesh);

	virtual Triangle* triangleTransform(Triangle* triangle, float parX, float parY, float parZ);
	virtual Triangle* triangleTransform(Triangle* triangle);

	virtual Point3D* pointTransform(Point3D* point, float parX, float parY, float parZ) = 0;
	virtual Point3D* pointTransform(Point3D* point);

	virtual Vertex* vertexTransform(Vertex* vertex, float parX, float parY, float parZ) = 0;
	virtual Vertex* vertexTransform(Vertex* vertex);

protected:
	float parX, parY, parZ;
};

