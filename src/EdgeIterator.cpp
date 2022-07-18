#include "EdgeIterator.h"

EdgeIterator::EdgeIterator(const Vertex* point) {
	reset(point);
}

EdgeIterator::EdgeIterator(const Triangle* triangle) {
	reset(triangle);
}


halfEdge* EdgeIterator::getNext() {

	halfEdge* next = nextEdge;

	if (nextEdge)
	{
		if (vertexIterator) // edges starting at a vertex iterator
		{
			halfEdge* twin = nextEdge->get_twinEdge();
			//assert(twin);
			nextEdge = twin->get_nextEdge();
		}
		else // edges from face iterator
		{
			nextEdge = nextEdge->get_nextEdge();
		}
	}

	if (nextEdge == startEdge)
	{
		nextEdge = NULL;
	}

	return next;
}

bool EdgeIterator::hasNext() const {
	return nextEdge != NULL;
}

void EdgeIterator::reset() {
	nextEdge = startEdge;
}

void EdgeIterator::reset(const Vertex* vertex) {
	vertexIterator = true;
	startEdge = vertex->getIncidentEdge();
	reset();
}

void EdgeIterator::goNextEdge()
{
	startEdge = nextEdge;
	nextEdge = startEdge->get_nextEdge();
}

void EdgeIterator::goTwinEdge()
{
	startEdge = startEdge->get_twinEdge();
	nextEdge = startEdge->get_nextEdge();
}

void EdgeIterator::goNext()
{
	startEdge = startEdge->get_twinEdge()->get_nextEdge();
	nextEdge = startEdge->get_twinEdge()->get_nextEdge();
}

halfEdge* EdgeIterator::get_StartEdge()
{
	return startEdge;
}

halfEdge* EdgeIterator::search_Higher(float z)
{
	return nullptr;
}

void EdgeIterator::circleThrough(float z) {

	halfEdge* current = startEdge;
	halfEdge* next = nullptr;
	halfEdge* prev = nullptr;

	do {
		next = current->get_nextEdge();
		prev = current->get_previousEdge();

		if ((prev->getOrigin()->getPoint().get_z() < z && prev->get_twinEdge()->getOrigin()->getPoint().get_z() > z)
			||
			(prev->getOrigin()->getPoint().get_z() > z && prev->get_twinEdge()->getOrigin()->getPoint().get_z() < z))
		{
			if (prev->get_Face()->get_zmin() < z)
			{
				current = prev->get_twinEdge();
			}
			else
			{
				current = next->get_twinEdge();
			}
		}
		else if ((next->getOrigin()->getPoint().get_z() < z && next->get_twinEdge()->getOrigin()->getPoint().get_z() > z)
			||
			(next->getOrigin()->getPoint().get_z() > z && next->get_twinEdge()->getOrigin()->getPoint().get_z() < z))
		{
			if (next->get_Face()->get_zmin() < z)
			{
				current = next->get_twinEdge();
			}
			else
			{
				current = prev->get_twinEdge();
			}
		}

	} while (current != startEdge && next != startEdge && prev != startEdge);

}

void EdgeIterator::circleStep(float z) {

	halfEdge* current = startEdge;
	halfEdge* next;
	halfEdge* prev;

	while (1)
	{
		current = current->get_twinEdge();
		next = current->get_nextEdge();
		prev = current->get_previousEdge();
		if ((next->getOrigin()->getPoint().get_z() < z && next->get_twinEdge()->getOrigin()->getPoint().get_z() > z)
			||
			(next->getOrigin()->getPoint().get_z() > z && next->get_twinEdge()->getOrigin()->getPoint().get_z() < z))
		{
		
			/*if (next->get_twinEdge()->get_Face()->get_zmin() > z)
			{*/
				current = next;
			//}
		}
		else if ((prev->getOrigin()->getPoint().get_z() < z && prev->get_twinEdge()->getOrigin()->getPoint().get_z() > z)
			||
			(prev->getOrigin()->getPoint().get_z() > z && prev->get_twinEdge()->getOrigin()->getPoint().get_z() < z))
		{
			
			//if (prev->get_twinEdge()->get_Face()->get_zmin() > z)
			//{
				current = prev;
			//}
		}
		if (current == startEdge)
		{
			current = startEdge->get_twinEdge();
		}
		else
			break;
	}
	startEdge = current;
	nextEdge = next;
	return ;
}
void EdgeIterator::reset(const Triangle* triangle) {

	vertexIterator = false;
	startEdge = triangle->get_boundary();
	reset();
}
