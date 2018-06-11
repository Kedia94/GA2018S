#include "graph.h"
#include <iostream>
#include <string.h>

Graph::Graph(int num): 
_numVertex(num), _numEdge(0)
{
	_edgeIdByVertexId = new int[MAX_VERTEX]();
	for (int i=0; i<MAX_VERTEX; i++)
		_edgeIdByVertex[i] = new struct edge_other[MAX_VERTEX]();
}
Graph::~Graph()
{
	delete[] _edgeIdByVertexId;
	for (int i=0; i<MAX_VERTEX; i++)
		delete[] _edgeIdByVertex[i];
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

	_edgeIdByVertex[v1][_edgeIdByVertexId[v1]++] = {v2, weight};

	_edgeIdByVertex[v2][_edgeIdByVertexId[v2]++] = {v1, weight};

	_numEdge++;

	return 0;
}
/*
int Graph::getWeight(int index)
{
	return _edge[index].weight;
}
*/

int Graph::getNumVertex()
{
	return _numVertex;
}

int Graph::getNumEdge()
{
	return _numEdge;
}

struct edge *Graph::getEdge(int index)
{
	return &_edge[index];
}
/*
struct edge *Graph::getEdgeByIndex(int v1, int v2)
{
	if (v1 >= v2)
	{
		int temp = v1;
e		v1 = v2;
				++_count;
		v1 = temp;
	}
		
	std::pair<int, int> key(v1, v2);
	return &_edgemap[key];
}
*/
struct edge_other * Graph::getEdgesByVertex(int v, int *ret)
{
	*ret = _edgeIdByVertexId[v];
	return _edgeIdByVertex[v];
}

void Graph::shakeEdge(int *list)
{
	struct edge tempEdge[MAX_EDGE];
	for (int i=0; i<_numEdge; ++i)
	{
		tempEdge[i] = _edge[list[i]];
	}
	memcpy(_edge, tempEdge, MAX_EDGE * sizeof(struct edge));
}
