#include "MeshParserStrategy.h"

MeshParserStrategy::MeshParserStrategy(std::string file, Mesh_DCEL* m)
{
	std::string fileTreated;

	for (int i = 0; i < file.size(); i++)
	{
		if (file.at(i) == 47)
			fileTreated.append("//");
		else fileTreated += file.at(i);
	}

	const char* f = fileTreated.c_str();

	if (isSTLFileASCII(f))
	{
		ASCIISTLMeshParser parser(f);
		auto start = std::chrono::high_resolution_clock::now();
		parser.parseFile(m);
		auto end = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double, std::milli> float_ms = end - start;
		std::cout << "Time Building ASCII DCEL: " << float_ms.count() << std::endl;
	}
	else
	{
		BinarySTLMeshParser parser(f);

		auto start = std::chrono::high_resolution_clock::now();
		parser.parseFile(m);
		auto end = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double, std::milli> float_ms = end - start;
		std::cout << "Time Building Binary DCEL: " << float_ms.count() << std::endl;
	}


}


MeshParserStrategy::~MeshParserStrategy(void)
{ }


bool MeshParserStrategy::isSTLFileASCII(const char* f)
{
	using namespace std;
	string line1, line2;
	ifstream input(f);

	if (!input.good()) return false;

	getline(input, line1);
	getline(input, line2);

	// Procura a palavra "facet" na segunda linha do arquivo. Caso encontre retorna 1, caso contr�rio retorna 0.
	if (line2.find("facet") != string::npos)
		return true;
	else return false;
}
