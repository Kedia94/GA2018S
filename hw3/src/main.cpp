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
#define GEN_SIZE 1000
#define EXISTED_SIZE 900
#define BEST_SIZE 10
#define MAX_GEN 100
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
	struct timeval t1, t2;
	gettimeofday(&t1, NULL);

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
//		std::cout<<"Repeat: "<<repeats<<std::endl;
		/* Part 1: Generate 1st generation */

		std::vector<Solution*> sol;
		int a[MAX_VERTEX];
		int sigma[MAX_VERTEX];
		int edgeSigma[MAX_EDGE];

		/* random sigma function */
		for (int i=0; i<vertex; ++i)
		{
			sigma[i] = i;
		}
		for (int i=0; i<edge; ++i)
		{
			edgeSigma[i] = i;
		}

		for (int i=0; i<vertex; ++i)
		{
			int temp = rand() % vertex;
			int swap_temp = sigma[i];
			sigma[i] = sigma[temp];
			sigma[temp] = swap_temp;
		}
		for (int i=0; i<edge; ++i)
		{
			int temp = rand() % edge;
			int swap_temp = edgeSigma[i];
			edgeSigma[i] = edgeSigma[temp];
			edgeSigma[temp] = swap_temp;
		}
		for (int i=0; i<GEN_SIZE; ++i)
		{
			for (int j=0; j<vertex; ++j)
				a[j] = rand()%2;

			Solution *temp_Sol = new Solution(vertex, a, &graph);
			temp_Sol->getValue(&graph);
			temp_Sol->localOptimize(&graph, vertex, edge, sigma, edgeSigma);
			sol.push_back(temp_Sol);
		}

		int valueSum[MAX_VERTEX];
		int sel1, sel2;

		/* Part 2: Mix generation */
		int first_max, max, max_i;
		int max_new, max_new_i;
		first_max = INT_MIN;
		for (int i=0; i<MAX_GEN; ++i)
		{
			std::vector<Solution*> now_sol;
			for (int i=0; i<GEN_SIZE; ++i)
			{
				sol[i]->getValue(&graph);
			}
			std::sort(sol.begin(), sol.end(), cmp);

			for (int j=0; j<BEST_SIZE; ++j)
			{
				now_sol.push_back(sol[j]);
			}

			for (int j=BEST_SIZE; j<EXISTED_SIZE; ++j)
			{
				/* select Parents */
				sel1 = rand() % ((1+GEN_SIZE)/2);

				sel2 = rand() % ((1+GEN_SIZE)/2);

				/* Crossover */
				for (int k=0; k<vertex; ++k)
					a[k] = rand()%2;
				Solution *temp_Sol = new Solution(vertex, sol[sel1], sol[sel2], a, &graph);
				temp_Sol->getValue(&graph);
				/* Mutate */
				for (int k=0; k<vertex; ++k)
				{
					if (rand() % MUTATE == 0)
					{
						temp_Sol->mutateKey(k, &graph);
						temp_Sol->flipKey(k, &graph);
					}
				}
				temp_Sol->localOptimize(&graph, vertex, edge, sigma, edgeSigma);
				now_sol.push_back(temp_Sol);
			}
			for (int j=EXISTED_SIZE; j<GEN_SIZE; ++j)
			{
				for (int k=0; k<vertex; ++k)
					a[k] = rand()%2;
				Solution *temp_Sol = new Solution(vertex, a, &graph);
				temp_Sol->getValue(&graph);
				temp_Sol->localOptimize(&graph, vertex, edge, sigma, edgeSigma);
				now_sol.push_back(temp_Sol);

			}

			max = INT_MIN;
			max_new = INT_MIN;
#ifdef PRINT_TIME
			int average = 0 ;
#endif
			for (int j=0; j<GEN_SIZE; ++j)
			{
#ifdef PRINT_TIME
				average += now_sol[j]->getValue(&graph);
#endif
				if (max_new < now_sol[j]->getValue(&graph))
				{
					max_new = now_sol[j]->getValue(&graph);
					max_new_i = j;
				}
			}
			if (first_max < max_new)
				first_max = max_new;
			max = max_new;
			max_i = max_new_i;

			for (int j=BEST_SIZE; j<GEN_SIZE; ++j)
			{
				delete sol[j];
			}
			sol.clear();
			sol.assign(now_sol.begin(), now_sol.end());
#ifdef PRINT_TIME
			gettimeofday(&t2, NULL);
//			std::cout<<average/(float)GEN_SIZE << " "<<(t2.tv_sec - t1.tv_sec) <<" secs "<<first_max <<" " <<max_new<<std::endl;
#endif
			if (repeats == 0 && t2.tv_sec - t1.tv_sec > 450)
				i += MAX_GEN; 
		}

		long _count = 0;
		long total_sum = 0;
		Solution *temp_sol;
		int max_index_s = INT_MIN;
		int max_value_s = INT_MIN;
		for (int ii=0; ii<GEN_SIZE;++ii)
		{
			total_sum += sol[ii]->getValue(&graph);
			if (max_value_s < sol[ii]->getValue(&graph))
			{
				max_value_s = sol[ii]->getValue(&graph);
				max_index_s = ii;
			}

		}
		temp_sol = sol[max_index_s];

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
		max_list[repeats] = temp_sol->getValue(&graph);

		for (int j=0; j<GEN_SIZE; ++j)
		{
			delete sol[j];
		}
		sol.clear();

		gettimeofday(&t2, NULL);
		if ((t2.tv_sec - t1.tv_sec) *(repeats+2) > 450 * (repeats+1))
			break;
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
	std::ofstream fw("maxcut.out");
	fw << result_list[max_index];
	fw.close();


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
