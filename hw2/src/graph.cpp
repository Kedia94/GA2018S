#include "graph.h"

Graph::Graph(int num): 
_numVertex(num), _numEdge(0)
{
}


int Graph::addEdge(int v1, int v2, int weight)
{
	if (v1 < v2)
	{
		_edge[_numEdge].v1 = v1;
		_edge[_numEdge].v2 = v2;
	}
	else
	{
		_edge[_numEdge].v1 = v2;
		_edge[_numEdge].v2 = v1;
	}
	_edge[_numEdge].weight = weight;
	std::pair<int, int> key(v1, v2);

	_edgemap[key] = _edge[_numEdge];


	_numEdge++;

	return 0;
}

int Graph::getWeight(int index)
{
	return _edge[index].weight;
}

int Graph::getNumVertex()
{
	return _numVertex;
}

int Graph::getNumEdge()
{
	return _numEdge;
}

struct edge Graph::getEdge(int index)
{
	return _edge[index];
}

struct edge Graph::getEdgeByIndex(int v1, int v2)
{
	if (v1 >= v2)
	{
		int temp = v1;
		v1 = v2;
		v1 = temp;
	}
		
	std::pair<int, int> key(v1, v2);
	return _edgemap[key];
}
