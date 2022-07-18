#pragma once
#include <sstream>
#include "Model.h"//To convert ASCII STLs into our internal model representation.

class StlAscii
{
public:

	StlAscii();
	~StlAscii();

	static float is_stl_ascii(const std::string& filename);

	static Model import(const std::string& filename);

	static std::stringstream import2(const std::string& filename);


protected:

	std::vector<std::vector<std::array<Point3D, 3>>> meshes;

	void load(const std::string& filename);

	Model to_model() const;
};