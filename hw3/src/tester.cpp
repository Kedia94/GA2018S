#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <vector>
#include <limits.h>
#include <string.h>

#include "graph.h"
#include "solution.h"

#define GEN_SIZE 500
#define MAX_GEN 1000
#define MUTATE (1 << 8)

#define PRINT_TIME
#ifdef PRINT_TIME
#include <sys/time.h>
#endif

bool cmp(Solution *a, Solution *b)
{
	return a->getValue() > b->getValue();
}

int main()
{
	std::srand(std::time(nullptr));
#ifdef PRINT_TIME
	struct timeval t1, t2;
	gettimeofday(&t1, NULL);
#endif		

	std::string line;
	std::ifstream maxin ("maxcut.in");

	if (!maxin.is_open()){
		std::cout << "There is no file: maxcut.in" << std::endl;
		exit(-1);
	}

	int vertex, edge, edge_iter;
	maxin >> vertex;
	maxin >> edge;
	edge_iter = 0;

	Graph graph(vertex);

	/* Add Graph Edge */
	while (edge_iter < edge)
	{
		int v1, v2, weight;
		maxin >> v1;
		maxin >> v2;
		maxin >> weight;
		--v1;
		--v2;

		graph.addEdge(v1, v2, weight);

		++edge_iter;
	}
	maxin.close();

	std::ifstream maxout ("maxcut.out");

	int k;
	int a[MAX_VERTEX];
	memset(a, 0, sizeof(a));
	while (maxout >> k)
	{
		a[--k] = 1;
	}
	/* Part 1: Generate 1st generation */

	std::vector<Solution*> sol;

	for (int i=0; i<GEN_SIZE; ++i)
	{
		sol.push_back(new Solution(vertex, a, &graph));
	}
	maxout.close();


#ifdef PRINT_TIME
	gettimeofday(&t2, NULL);
	std::cout<<"Result: "<<sol[0]->getValue(&graph)<<std::endl;
	std::cout<<"Spent: "<<((t2.tv_sec-t1.tv_sec)*1000 + (t2.tv_usec-t1.tv_usec)*0.001f) <<" msecs"<<std::endl;
#endif
	return 0;
}
