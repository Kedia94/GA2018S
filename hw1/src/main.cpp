#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <vector>

#include "graph.h"
#include "solution.h"

#define PRINT_TIME
#define GEN_SIZE 500
#define MAX_GEN 1000
#define MUTATE (1 << 8)

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
	std::ifstream maxin ("weighted_500.txt");

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

	/* Part 1: Generate 1st generation */

	std::vector<Solution*> sol;
	int a[MAX_VERTEX];

	for (int i=0; i<GEN_SIZE; ++i)
	{
		for (int j=0; j<vertex; ++j)
			a[j] = rand()%2;
		sol.push_back(new Solution(vertex, a));
	}

	int valueSum[MAX_VERTEX];
	int sel1, sel2;

	/* Part 2: Mix generation */
	int first_max, max, max_i;
	int max_new, max_new_i;
	for (int i=0; i<MAX_GEN; ++i)
	{
		std::vector<Solution*> now_sol;
		for (int i=0; i<GEN_SIZE; ++i)
			sol[i]->getValue(graph);

		std::sort(sol.begin(), sol.end(), cmp);

		for (int j=0; j<GEN_SIZE; ++j)
		{
			/* select Parents */
			sel1 = rand() % (GEN_SIZE/4);
	
			sel2 = rand() % (GEN_SIZE/4);

			/* Crossover */
			for (int k=0; k<vertex; ++k)
				a[k] = rand()%2;
			now_sol.push_back(new Solution(vertex, sol[sel1], sol[sel2], a));

			/* Mutate */
			for (int k=0; k<vertex; ++k)
			{
				if (rand() % MUTATE == 0)
				{
					now_sol[j]->mutateKey(k);
				}
			}
		}

		max = 0;
		max_new = 0;
		for (int j=0; j<GEN_SIZE; ++j)
		{
			if (max_new < now_sol[j]->getValue(graph))
			{
				max_new = now_sol[j]->getValue(graph);
				max_new_i = j;
			}
		}
		if (first_max < max_new)
			first_max = max_new;
		max = max_new;
		max_i = max_new_i;

		for (int j=0; j<GEN_SIZE; ++j)
		{
			delete sol[j];
		}
		sol.clear();
		sol.assign(now_sol.begin(), now_sol.end());
		if (i >= MAX_GEN/4)
		{
			if (first_max <= max_new)
			{
				break;
			}
		}
	}

	int initial = 0;
	for (int i=0; i<vertex; ++i)
	{
		if (sol[max_i]->isInclude(i) == 1)
		{
			if (initial == 0)
			{
				++initial;
				std::cout<<(i+1);
			}
			else
				std::cout<<" "<<(i+1);
		}
	}
	std::cout<<std::endl;


#ifdef PRINT_TIME
	gettimeofday(&t2, NULL);
	std::cout<<"Result: "<<sol[max_i]->getValue(graph)<<std::endl;
	std::cout<<"Spent: "<<((t2.tv_sec-t1.tv_sec)*1000 + (t2.tv_usec-t1.tv_usec)*0.001f) <<" msecs"<<std::endl;
#endif
    return 0;
}
