#pragma once

#include "MeshParser.h"

class  BinarySTLMeshParser : public MeshParser
{
public:
	BinarySTLMeshParser(std::string f);
	~BinarySTLMeshParser(void);

	void parseFile(Mesh_DCEL* m);
};