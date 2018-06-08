#include "solution.h"

#include <iostream>

Solution::Solution(int size, int *array):
_size(size), _calculated(0), _value(0)
{
	_exists = new int[size];
	for (int i=0; i<size; ++i)
		_exists[i] = array[i];
}

Solution::Solution(int size, Solution* sol1, Solution* sol2, int *randarray):
_size(size), _calculated(0), _value(0)
{
	_exists = new int[size];
	for (int i=0; i<size; ++i)
	{
		if (randarray[i] == 0)
			_exists[i] = sol1->isInclude(i);
		else
			_exists[i] = sol2->isInclude(i);
	}
}

Solution::~Solution()
{
	delete _exists;
}

int Solution::isInclude(int key)
{
	return _exists[key];
}

int Solution::mutateKey(int key, Graph* graph)
{
	int changed = 0;

	int state = 1 - _exists[key];
	int index;
	struct edge_other *tempEdges = (struct edge_other *)graph->getEdgesByVertex(key, &index);
	for (int i=0; i<index; ++i)
	{
		struct edge_other* tempEdge = tempEdges+i;
		int otherVertex = tempEdge->v2;
	
		changed -= (1 - 2*(state ^ _exists[otherVertex])) * tempEdge->weight;
	}

	_recentChange = changed;
	return changed;
}

void Solution::flipKey(int key)
{
	_exists[key] = 1 - _exists[key];
	_value += _recentChange;
}

int Solution::mutateEdge(int edgeNum, Graph *graph)
{

	struct edge* tempEdge;
	tempEdge = graph->getEdge(edgeNum);
	_recentChangeEdge = mutateKey(tempEdge->v1, graph);
	_recentChangeEdge += mutateKey(tempEdge->v2, graph);

	_recentChangeEdge -= (2 - 4*(_exists[tempEdge->v1] ^ _exists[tempEdge->v2])) * tempEdge->weight;
	
	return _recentChangeEdge;
}

void Solution::flipEdge(int edgeNum, Graph *graph)
{
	_exists[graph->getEdge(edgeNum)->v1] = 1 - _exists[graph->getEdge(edgeNum)->v1];
	_exists[graph->getEdge(edgeNum)->v2] = 1 - _exists[graph->getEdge(edgeNum)->v2];
	_value += _recentChangeEdge;
}

int Solution::getValue(Graph* graph)
{
	if (_calculated)
		return _value;

	_calculated = 1;

	int edgeNum = graph->getNumEdge();

	for (int i=0; i<edgeNum; ++i)
	{
		struct edge* edg = graph->getEdge(i);
		if (_exists[edg->v1] + _exists[edg->v2] == 1)
			_value += edg->weight;
	}

	return _value;
}

int Solution::getValue()
{
	return _value;
}

void Solution::print()
{
	for (int i=0; i<_size; i++)
		if (_exists[i])
			std::cout<<i<<" ";
	std::cout<<std::endl;
}
