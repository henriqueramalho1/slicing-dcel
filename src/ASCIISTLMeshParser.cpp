#include "ASCIISTLMeshParser.h"
#include "Model.h"
#include "stl_ascii.h"

ASCIISTLMeshParser::ASCIISTLMeshParser(std::string f) : MeshParser(f)
{ }

ASCIISTLMeshParser::~ASCIISTLMeshParser(void)
{ }

void ASCIISTLMeshParser::parseFile(Mesh_DCEL* m)
{
	Model model;
	model = StlAscii::import(file);
	buildMesh(m, model);
}