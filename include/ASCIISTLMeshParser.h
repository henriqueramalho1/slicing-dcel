#pragma once

#include <chrono>
#include <fstream>
#include <stdexcept>
#include <string>
#include "MeshParser.h"

class  ASCIISTLMeshParser : public MeshParser
{
public:
	ASCIISTLMeshParser(std::string f);
	~ASCIISTLMeshParser(void);

	void parseFile(Mesh_DCEL* m);
};
