#ifndef _SOLUTION_H_
#define _SOLUTION_H_
#include "graph.h"

#include <iostream>
class Solution
{
private:
	int _size;
	int *_exists;
	int _calculated;
	int _recentChange;
	int _recentChangeEdge;
	int *_vertexValue;
	void _calculateVertexChange(Graph *graph);
	int _value;

public:
	Solution(int size, int *array, Graph *graph);
	Solution(int size, Solution* sol1, Solution* sol2, int *randarray, Graph *graph);
	~Solution();
	int isInclude(int key);
	int mutateKey(int key, Graph* graph);
	void flipKey(int key, Graph* graph);
	int mutateEdge(int edgeNum, Graph* graph);
	void flipEdge(int edgeNum, Graph* graph);
	int getValue(Graph* graph);
	int getValue();
	void localOptimize(Graph* graph, int vertex, int edge, int *sigma, int *edgeSigma);
	void print();

	bool operator<(const Solution &a) const {
			std::cout<<_value<<" < "<<a._value<<std::endl;
			return _value < a._value;
	}
	
};

#endif
