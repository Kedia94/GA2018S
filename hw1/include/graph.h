#include <map>
#define MAX_VERTEX 1000

class Graph
{
private:
	int _numVertex, _numEdge;
	std::map<int, int> _edge; // key : large * MAX_VERTEX + small

public:
	Graph(int num);
	int GetKey(int v1, int v2);
	int AddEdge(int v1, int v2, int weight);
	int GetWeight(int v1, int v2);
	int GetNumVertex();
};
