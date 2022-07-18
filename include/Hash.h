#pragma once
#include <iostream>

template<class TYPE>
class Hash
{
private:

	class Node
	{
	private:
		TYPE pElement;
		Node* next;
		Node* prev;

	public:
		Node(TYPE element);
		~Node();
		void setNext(Node* n);
		void setPrev(Node* n);
		Node* getNext() const;
		Node* getPrev() const;
		void setElement(TYPE element);
		TYPE getElement() const;
	};

	int keys;
	int elements;
	Node** list;
	Node* iterator;

public:

	Hash();
	~Hash();
	void setHashKeys(int n);
	int insert(TYPE element, int index);
	TYPE remove(TYPE element, int index);
	void remove(Node* n);
	void clear();
	bool isEmpty() const;
	void initList();
	int getSize() const;
	const int getKeys() const;
	void initIterator(int index);
	void iteratorGoNext();
	TYPE getIteratorElement() const;
};

template<class TYPE>
Hash<TYPE>::Node::Node(TYPE element) :
	pElement(element),
	next(NULL),
	prev(NULL)
{

}

template<class TYPE>
Hash<TYPE>::Node::~Node()
{
	next = NULL;
	prev = NULL;
}

template<class TYPE>
void Hash<TYPE>::Node::setNext(Node* n)
{
	next = n;
}

template<class TYPE>
void Hash<TYPE>::Node::setPrev(Node* n)
{
	prev = n;
}

template<class TYPE>
typename Hash<TYPE>::Node* Hash<TYPE>::Node::getNext() const
{
	return next;
}

template<class TYPE>
typename Hash<TYPE>::Node* Hash<TYPE>::Node::getPrev() const
{
	return prev;
}

template<class TYPE>
void Hash<TYPE>::Node::setElement(TYPE element)
{
	pElement = element;
}

template<class TYPE>
TYPE Hash<TYPE>::Node::getElement() const
{
	return pElement;
}

template<class TYPE>
Hash<TYPE>::Hash():
	keys(-1),
	list(NULL),
	elements(0),
	iterator(NULL)
{
	if (keys == 0)
	{
		std::cout << "[ERROR]: cannot create an 0 key hash" << std::endl;
		std::exit(2);
	}
}

template<class TYPE>
Hash<TYPE>::~Hash()
{
	iterator = NULL;
	clear();
}

template<class TYPE>
void Hash<TYPE>::setHashKeys(int n)
{
	keys = n;
	initList();
}

template<class TYPE>
int Hash<TYPE>::insert(TYPE element, int index)
{
	bool NoCollision = true;

	if (element == NULL)
	{
		std::cout << "[ERROR]: cannot insert element in hash table" << std::endl;
		std::exit(9);
	}

	if (index < 0 || index >= keys)
	{
		std::cout << "[ERROR]: cannot insert element out of hash table" << std::endl;
		std::exit(9);
	}

	if (list[index] != NULL)
	{
		NoCollision = false;
	}

	Node* node = new Node(element);

	node->setNext(list[index]);

	if(list[index] != NULL)
		list[index]->setPrev(node);

	list[index] = node;
	node->setPrev(NULL);

	elements++;

	return NoCollision;
}

template<class TYPE>
TYPE Hash<TYPE>::remove(TYPE element, int index)
{
	Node* node = list[index];
	
	if (node == NULL)
	{
		std::cout << "[ERROR]: cannot remove element in empty index in the hash" << std::endl;
		std::exit(8);
	}

	while (element != node->getElement())
	{
		node = node->getNext();
	}

	TYPE pElement = node->getElement();
	node->setElement(NULL);

	if (node == list[index])
	{
		list[index] = node->getNext();
	}
	else if (node->getNext() == NULL)
	{
		node->getPrev()->setNext(NULL);
	}
	else
	{
		node->getPrev()->setNext(node->getNext());
	}

	delete node;
	elements--;

	return pElement;
}

template<class TYPE>
void Hash<TYPE>::clear()
{
	Node* node = NULL;

	for (int i = 0; i < keys; i++)
	{
		while (list[i] != NULL)
		{
			node = list[i]->getNext();
			delete list[i];
			list[i] = node;
			elements--;
		}
	}

	keys = 0;
	iterator = NULL;
}

template<class TYPE>
bool Hash<TYPE>::isEmpty() const
{
	for (int i = 0; i < keys; i++)
	{
		if (!list[i])
			return false;
	}

	return true;
}

template<class TYPE>
void Hash<TYPE>::initList()
{
	if (keys <= 0)
	{
		std::cout << "[ERROR] Cannot init 0 sized hash" << std::endl;
		std::exit(2);
	}

	list = new Node* [keys];

	for (int i = 0; i < keys; i++)
	{
		list[i] = NULL;
	}
}

template<class TYPE>
int Hash<TYPE>::getSize() const
{
	return elements;
}

template<class TYPE>
const int Hash<TYPE>::getKeys() const
{
	return keys;
}

template<class TYPE>
void Hash<TYPE>::initIterator(int index)
{
	iterator = list[index];
}

template<class TYPE>
void Hash<TYPE>::iteratorGoNext()
{
	if (iterator == NULL)
	{
		std::cout << "[ERROR]: cannot access the list without initializing iterator" << std::endl;
		std::exit(8);
	}

	iterator = iterator->getNext();
}

template<class TYPE>
TYPE Hash<TYPE>::getIteratorElement() const
{
	if (iterator == NULL)
		return NULL;

	return iterator->getElement();
}