#pragma once

#include <fstream>
#include <string>
#include "ASCIISTLMeshParser.h"
#include "BinarySTLMeshParser.h"
#include "MeshParser.h"


class  MeshParserStrategy
{
public:
	MeshParserStrategy(std::string file, Mesh_DCEL* m);
	~MeshParserStrategy(void);

private:
	bool isSTLFileASCII(const char* f);
};
