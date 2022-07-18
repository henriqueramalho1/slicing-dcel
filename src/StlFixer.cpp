#include "StlFixer.h"
#include "math_functions.h"

StlFixer::StlFixer():
	hash()
{
}

StlFixer::~StlFixer()
{
}

StlFixer* StlFixer::getInstance()
{
	if (instance == NULL)
	{
		instance = new StlFixer();
	}

	return instance;
}

void StlFixer::roundPoint(float* v1, float* v2, float* v3)
{
	*v1 = roundIt(*v1, 1000000);
	*v2 = roundIt(*v2, 1000000);
	*v3 = roundIt(*v3, 1000000);
}

void StlFixer::checkDuplicates(std::vector<std::array<Point3D, 3>> *triangles)
{
	initHash(triangles->size());

	for (int i = 0; i < triangles->size(); i++)
	{
		std::array<Point3D, 3> triangle = (*triangles)[i];

		for (int j = 0; j < 3; j++)
		{
			Point3D p = triangle[j];

			if (hash.getHashIndex(p))
			{
				//std::cout << "Inserting in index [" << hash.getHashIndex(p) << "] Point<" << p.get_x() << ", " << p.get_y() << ", " << p.get_z() << ">" << std::endl;
				if (!hash.insert(p))
				{
					//std::cout << std::endl << "COLISAO [" << hash.getHashIndex(p1) << "] Point<" << p1.get_x() << ", " << p1.get_y() << ", " << p1.get_z() << ">" << std::endl;

					hash.initIterator(hash.getHashIndex(p));
					hash.iteratorGoNext();

					Point3D pAdj1;
					Point3D pAdj2;

					if (j == 0)
					{
						pAdj1 = triangle[1];
						pAdj2 = triangle[2];
					}
					else if (j == 1)
					{
						pAdj1 = triangle[0];
						pAdj2 = triangle[2];
					}
					else if (j == 2)
					{
						pAdj1 = triangle[0];
						pAdj2 = triangle[1];
					}

					while (hash.getIteratorElement() != NULL)
					{
						Point3D pIn = hash.getIteratorElement();

						if (calcDistance(p, pIn) < 0.0001f && pIn != pAdj1 && pIn != pAdj2)
						{
							hash.remove(p);
							((*triangles)[i])[j].set_x(pIn.get_x());
							((*triangles)[i])[j].set_y(pIn.get_y());
							((*triangles)[i])[j].set_z(pIn.get_z());

							break;
						}

						hash.iteratorGoNext();
					}
				}

			}
		}
		
	}

	hash.clearHash();
}

float StlFixer::calcDistance(Point3D p1, Point3D p2)
{
	float distance;

	distance = sqrtf((p1.get_x() - p2.get_x()) * (p1.get_x() - p2.get_x()) + (p1.get_y() - p2.get_y()) * (p1.get_y() - p2.get_y()) + (p1.get_z() - p2.get_z()) * (p1.get_z() - p2.get_z()));

	return distance;
}

int StlFixer::serchPrime(int n)
{
	int find = 0;

	while (find == 0)
	{
		find = 1;
		int i = 2;

		while (i * i <= n)
		{
			if (n % i == 0)
			{
				n++;
				find = 0;
				break;
			}

			i++;
		}

	}

	return n;
}

void StlFixer::showHash()
{
	std::cout << std::endl << std::endl << "SHOWING HASH" << std::endl;

	for (int i = 0; i < hash.getKeys(); i++)
	{
		hash.initIterator(i);

		while (hash.getIteratorElement() != NULL)
		{
			Point3D p = hash.getIteratorElement();
			std::cout << "[" << hash.getHashIndex(p) << "] Point<" << p.get_x() << ", " << p.get_y() << ", " << p.get_z() << ">" << std::endl;
			hash.iteratorGoNext();
		}
	}
}

void StlFixer::initHash(int n)
{
	hash.initHash(serchPrime(n));
}

void StlFixer::clearHash()
{
	hash.clearHash();
}

StlFixer* StlFixer::instance = NULL;