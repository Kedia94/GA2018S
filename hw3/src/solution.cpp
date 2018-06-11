#include "solution.h"

#include <iostream>
#include <cstring>

Solution::Solution(int size, int *array, Graph* graph):
_size(size), _calculated(0), _value(0)
{
	_exists = new int[size];
	_vertexValue = new int[MAX_VERTEX];
	for (int i=0; i<size; ++i)
		_exists[i] = array[i];
	_calculateVertexChange(graph);
}

Solution::Solution(int size, Solution* sol1, Solution* sol2, int *randarray, Graph* graph):
_size(size), _calculated(0), _value(0)
{
	_exists = new int[size];
	_vertexValue = new int[MAX_VERTEX];
	for (int i=0; i<size; ++i)
	{
		if (randarray[i] == 0)
			_exists[i] = sol1->isInclude(i);
		else
			_exists[i] = sol2->isInclude(i);
	}
	_calculateVertexChange(graph);
}

Solution::~Solution()
{
	delete _exists;
	delete[] _vertexValue;
}

void Solution::_calculateVertexChange(Graph *graph)
{
	memset(_vertexValue, 0, MAX_VERTEX * sizeof(int));
	struct edge *tempEdge;
	int num = graph->getNumEdge();
	for (int i=0; i<num; ++i)
	{
		tempEdge = graph->getEdge(i);
		_vertexValue[tempEdge->v1] += tempEdge->weight * (1-2*(_exists[tempEdge->v1] ^ _exists[tempEdge->v2]));
		_vertexValue[tempEdge->v2] += tempEdge->weight * (1-2*(_exists[tempEdge->v1] ^ _exists[tempEdge->v2]));
	}
}

int Solution::isInclude(int key)
{
	return _exists[key];
}

int Solution::mutateKey(int key, Graph* graph)
{
	_recentChange = _vertexValue[key];
	return _vertexValue[key];
}

void Solution::flipKey(int key, Graph* graph)
{
	_exists[key] = 1 - _exists[key];
	_value += _recentChange;

	int state = _exists[key];
	int index;
	struct edge_other *tempEdges = graph->getEdgesByVertex(key, &index);
	for (int i=0; i<index; ++i)
	{
		struct edge_other* tempEdge = tempEdges+i;
		int otherVertex = tempEdge->v2;
	
		_vertexValue[otherVertex] += (2 - 4*(state ^ _exists[otherVertex])) * tempEdge->weight;
	}
		_vertexValue[key] = -_vertexValue[key];
}

int Solution::mutateEdge(int edgeNum, Graph *graph)
{
	struct edge* tempEdge;
	tempEdge = graph->getEdge(edgeNum);
	_recentChangeEdge = _vertexValue[tempEdge->v1];
	_recentChangeEdge += _vertexValue[tempEdge->v2];

	_recentChangeEdge -= (2 - 4*(_exists[tempEdge->v1] ^ _exists[tempEdge->v2])) * tempEdge->weight;
	return _recentChangeEdge;
}

void Solution::flipEdge(int edgeNum, Graph *graph)
{
	_value += _recentChangeEdge;
	int v1 = graph->getEdge(edgeNum)->v1;
	int v2 = graph->getEdge(edgeNum)->v2;
	_exists[v1] = 1 - _exists[v1];

	int state = _exists[v1];
	int index;
	struct edge_other *tempEdges = graph->getEdgesByVertex(v1, &index);
	for (int i=0; i<index; ++i)
	{
		struct edge_other* tempEdge = tempEdges+i;
		int otherVertex = tempEdge->v2;
	
		_vertexValue[otherVertex] += (2 - 4*(state ^ _exists[otherVertex])) * tempEdge->weight;
	}
		_vertexValue[v1] = -_vertexValue[v1];

	_exists[v2] = 1 - _exists[v2];
	state = _exists[v2];
	tempEdges = graph->getEdgesByVertex(v2, &index);
	for (int i=0; i<index; ++i)
	{
		struct edge_other* tempEdge = tempEdges+i;
		int otherVertex = tempEdge->v2;
	
		_vertexValue[otherVertex] += (2 - 4*(state ^ _exists[otherVertex])) * tempEdge->weight;
	}
		_vertexValue[v2] = -_vertexValue[v2];
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

void Solution::localOptimize(Graph* graph, int vertex, int edge, int* sigma, int* edgeSigma)
{
	int check_total = vertex + edge;
	int improved = check_total;
	int j=0, jj;
	while (improved)
	{
		--improved;
		j = (j+1) % check_total;
		jj = j - vertex;
		if (j<vertex && mutateKey(sigma[j], graph) > 0)
		{
			flipKey(sigma[j], graph);
			improved = check_total;
		}
		else if (jj>=0 && mutateEdge(edgeSigma[jj], graph) > 0)
		{
			flipEdge(edgeSigma[jj], graph);
			improved = check_total;
		}
	}

}

void Solution::print()
{
	for (int i=0; i<_size; i++)
		if (_exists[i])
			std::cout<<i<<" ";
	std::cout<<std::endl;
}
