#include "Mesh_DCEL.h"

Mesh_DCEL::Mesh_DCEL():
	upper_right_vertex(-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max(), -std::numeric_limits<float>::max()),
	bottom_left_vertex(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max())
{

}

Mesh_DCEL::~Mesh_DCEL()
{
	this->clear();
}

void Mesh_DCEL::round_vertices()
{
	double eps = 0.004;

	for(size_t i = 0; i < vertices.size(); i++)
	{
		Point3D p = vertices[i].get_point();

		p.set_x(xround(p.get_x(), eps, 2, 0));
		p.set_y(xround(p.get_y(), eps, 2, 0));
		p.set_z(xround(p.get_z(), eps, 2, 0));

		vertices[i].set_point(p);
	}
}

float Mesh_DCEL::xround (float x, double eps, int mod, int rem) {
  	double y = round((double)x/(mod * eps));
  	double z = (y * mod + rem) * eps;
  	return (float)z;
}

std::vector<Triangle>& Mesh_DCEL::get_faces()
{
	return this->faces;
}

std::vector<std::vector<int>>& Mesh_DCEL::get_face_indices()
{
	return this->face_indices;
}

std::vector<Vertex>& Mesh_DCEL::get_vertices()
{
	return this->vertices;
}

std::vector<HalfEdge>& Mesh_DCEL::get_halfedges()
{
	return this->edges;
}

void Mesh_DCEL::set_bottom_left_vertex(const Point3D& p)
{
	bottom_left_vertex = p;
}

void Mesh_DCEL::set_upper_right_vertex(const Point3D& p)
{
	upper_right_vertex = p;
}

Point3D Mesh_DCEL::get_bottom_left_vertex() const
{
	return bottom_left_vertex;
}

Point3D Mesh_DCEL::get_upper_right_vertex() const
{
	return upper_right_vertex;
}

void Mesh_DCEL::set_upper_bottom_vertices(Point3D val)
{
	if (val.get_x() < bottom_left_vertex.get_x())
		bottom_left_vertex.set_x(val.get_x());

	if (val.get_y() < bottom_left_vertex.get_y())
		bottom_left_vertex.set_y(val.get_y());

	if (val.get_z() < bottom_left_vertex.get_z())
		bottom_left_vertex.set_z(val.get_z());

	if (val.get_x() > upper_right_vertex.get_x())
		upper_right_vertex.set_x(val.get_x());

	if (val.get_y() > upper_right_vertex.get_y())
		upper_right_vertex.set_y(val.get_y());

	if (val.get_z() > upper_right_vertex.get_z())
		upper_right_vertex.set_z(val.get_z());
}

void Mesh_DCEL::update_triangles()
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

Point3D Mesh_DCEL::AABB_size()
{
	 return Point3D (upper_right_vertex.get_x() - bottom_left_vertex.get_x(), 
                    upper_right_vertex.get_y() - bottom_left_vertex.get_y(), 
                    upper_right_vertex.get_z() - bottom_left_vertex.get_z());
}