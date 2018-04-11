#include "graph.h"

Graph::Graph(int num): 
_numVertex(num), _numEdge(0)
{
}

int Graph::GetKey(int v1, int v2)
{
	return (v1 > v2) ? v1 * MAX_VERTEX + v2 : v2 * MAX_VERTEX + v1;
}

int Graph::AddEdge(int v1, int v2, int weight)
{
	int key = GetKey(v1, v2);

	_edge[key] = weight;
	_numEdge++;

	return 0;
}

int Graph::GetWeight(int v1, int v2)
{
	return _edge[GetKey(v1, v2)];
}

int Graph::GetNumVertex()
{
	return _numVertex;
}
