#pragma warning(disable : 4996)
#include "BinarySTLMeshParser.h"
#include <set>
#include "Model.h"
#include "stl_binary.h"


BinarySTLMeshParser::BinarySTLMeshParser(std::string f): MeshParser(f)
{ }

BinarySTLMeshParser::~BinarySTLMeshParser(void)
{ }

void BinarySTLMeshParser::parseFile(Mesh_DCEL *m)
{
	Model model;
	model = StlBinary::import(file);
	buildMesh(m, model);
}