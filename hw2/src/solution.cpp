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
		switch (randarray[i])
		{
			case 0: _exists[i] = sol1->isInclude(i); break;
			case 1: _exists[i] = sol2->isInclude(i); break;
		}
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

void Solution::mutateKey(int key, Graph graph)
{
	if (!_calculated)
		getValue(graph);

	_exists[key] = 1 - _exists[key];

	for (int i=0; i<_size; ++i)
	{
		if (i == key)
			continue;
		if (_exists[key] == _exists[i])
			_value -= graph.getEdgeByIndex(i, key).weight;
		else
			_value += graph.getEdgeByIndex(i, key).weight;
	}
}

int Solution::getValue(Graph graph)
{
	if (_calculated)
		return _value;

	_calculated = 1;

	int edgeNum = graph.getNumEdge();

	for (int i=0; i<edgeNum; ++i)
	{
		struct edge edg = graph.getEdge(i);
		if (isInclude(edg.v1) + isInclude(edg.v2) == 1)
			_value += edg.weight;
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
