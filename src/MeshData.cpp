#include "MeshData.h"

MeshData::MeshData(int i):
	id(i)
{
}

MeshData::~MeshData()
{
}

int MeshData::idx()
{
	return id;
}
