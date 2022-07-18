#pragma once

#include <string> //To accept filenames.
#include <array>
#include "Model.h"//To construct 3D models from the file.

class StlBinary 
{
public:

	StlBinary();
	~StlBinary();

	static float is_stl_binary(const std::string& filename);

	static Model import(const std::string& filename);

protected:

	std::vector<std::array<Point3D, 3>>triangles;

	void load(const std::string& filename);

	Model to_model() const;
};