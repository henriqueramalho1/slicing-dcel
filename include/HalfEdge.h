#pragma once
#include <iostream>


class Triangle;
class Vertex;

class halfEdge
{

protected:

	Vertex* origin;
	Triangle* face;
	halfEdge* nextEdge, * previousEdge, * twin;

public:

	halfEdge(Vertex* start);

	halfEdge();

	~halfEdge();

	//retorna o vertice de origem
	Vertex* get_startVertex() const;
	//retorna a face 
	Triangle* get_Face() const;
	//retorna a pr�xima aresta 
	halfEdge* get_nextEdge() const;
	//retorna a aresta anterior
	halfEdge* get_previousEdge() const;
	//retorna seu edge gemeo
	halfEdge* get_twinEdge() const;
	//retorna sua origem
	Vertex* getOrigin() const;


	//modifica a origem
	void set_startVertex(Vertex* point);
	//modifica a face 
	void set_Face(Triangle* face);
	//modifica a pr�xima aresta 
	void set_nextEdge(halfEdge* edge);
	//modifica a aresta anterior 
	void set_previousEdge(halfEdge* edge);
	//modifica o gemeo
	void set_twinEdge(halfEdge* edge);
	//seta a origem
	void setOrigin(Vertex* o);

};