#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <map>
#include <utility>
#define MAX_VERTEX 1500
#define MAX_EDGE 15000

struct edge
{
	int v1;
	int v2;
	int weight;
};

class Graph
{
private:
	int _numVertex, _numEdge;
	struct edge _edge[MAX_EDGE];
	std::map<std::pair<int, int>, struct edge> _edgemap;


public:
	Graph(int num);
	int addEdge(int v1, int v2, int weight);
	int getWeight(int index);
	int getNumVertex();
	int getNumEdge();
	struct edge getEdge(int index);
	struct edge getEdgeByIndex(int v1, int v2);
};

#endif
