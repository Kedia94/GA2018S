#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <map>
#include <utility>
#include <vector>
#define MAX_VERTEX 3000
#define MAX_EDGE 24000

struct edge
{
	int v1;
	int v2;
	int weight;
};

struct edge_other
{
	int v2;
	int weight;
};

class Graph
{
private:
	int _numVertex, _numEdge;
	struct edge _edge[MAX_EDGE];
	std::map<std::pair<int, int>, struct edge> _edgemap;
	struct edge_other *_edgeIdByVertex[MAX_VERTEX];
	int *_edgeIdByVertexId;


public:
	Graph(int num);
	~Graph();
	int addEdge(int v1, int v2, int weight);
	int getWeight(int index);
	int getNumVertex();
	int getNumEdge();
	struct edge *getEdge(int index);
//	struct edge *getEdgeByIndex(int v1, int v2);
	struct edge_other * getEdgesByVertex(int v, int *ret);
	void shakeEdge(int *list);
};

#endif
