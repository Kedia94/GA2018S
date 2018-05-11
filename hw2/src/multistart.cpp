#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <vector>
#include <limits.h>
#include <array>

#include "graph.h"
#include "solution.h"

#define REPEAT 1
#define GEN_SIZE 50
#define MAX_GEN 50
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

	/* Part 0: Prepare for repeat running */
	int max_list[REPEAT];
	std::string result_list[REPEAT];

	for (int repeats = 0; repeats < REPEAT; repeats++)
	{
		/* Part 1: Generate 1st generation */

		std::vector<Solution*> sol;
		int a[MAX_VERTEX];
		int sigma[MAX_VERTEX];

		/* random sigma function */
		for (int i=0; i<vertex; ++i)
		{
			sigma[i] = i;
		}

		for (int i=0; i<vertex; ++i)
		{
			int temp = rand() % vertex;
			int swap_temp = sigma[i];
			sigma[i] = sigma[temp];
			sigma[temp] = swap_temp;
		}
		for (int i=0; i<1; ++i)
		{
			for (int j=0; j<vertex; ++j)
				a[j] = rand()%2;

			sol.push_back(new Solution(vertex, a));
		

			Solution *temp_sol = new Solution(vertex, a);
			int improved = 1;
			while (improved)
			{
				improved = 0;
				for (int j=0; j<vertex; ++j)
				{
					int before = temp_sol->getValue(graph);
					temp_sol->mutateKey(sigma[j], graph);
					if (temp_sol->getValue(graph) > before)
					{
						improved = 1;
					}
					else
					{
						/* restore */
						temp_sol->mutateKey(sigma[j], graph);
					}
				}
			}
		
		int initial = 0;
		for (int i=0; i<vertex; ++i)
		{
			if (temp_sol->isInclude(i) == 1)
			{
				if (initial == 0)
				{
					++initial;
					result_list[repeats] += std::to_string(i+1);
				}
				else
					result_list[repeats] += " " + std::to_string(i+1);
			}
		}
		
		max_list[repeats] = temp_sol->getValue(graph);
		}
	}

	int max_index = -1;
	int max = INT_MIN;
	for (int i=0; i< REPEAT; i++)
	{
		if (max < max_list[i])
		{
			max = max_list[i];
			max_index = i;
		}
	}


#ifdef PRINT_TIME
	gettimeofday(&t2, NULL);
//	for (int i=0; i<REPEAT; i++)
//	{
		std::cout<<"Result: "<<max<<std::endl;//<< max_list[i] <<": "<<result_list[i]<<std::endl<<std::endl;;//sol[max_i]->getValue(graph)<<std::endl;
//	}
	std::cout<<"Spent: "<<((t2.tv_sec-t1.tv_sec)*1000 + (t2.tv_usec-t1.tv_usec)*0.001f) <<" msecs"<<std::endl;

#endif
	return 0;
}
