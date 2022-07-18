#pragma once

#include "Mesh_DCEL.h"
#include "Triangle.h"
#include "Model.h"
#include <sstream>

class  MeshParser
{
public:
	MeshParser(void);
	MeshParser(std::string f);
	~MeshParser(void);

	long getTimeBuld() const;
	virtual void parseFile(Mesh_DCEL* m) = 0;
	static void buildMesh(Mesh_DCEL* m, Model& model);

protected:
	std::string file;
};
