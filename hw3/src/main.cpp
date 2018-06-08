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
	std::cout<<"graph created"<<std::endl;
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
	std::cout<<"Edge Added"<<std::endl;

	/* Part 0: Prepare for repeat running */
	int max_list[REPEAT];
	std::string result_list[REPEAT];

	for (int repeats = 0; repeats < REPEAT; repeats++)
	{
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

			sol.push_back(new Solution(vertex, a));
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

			for (int j=0; j<GEN_SIZE; ++j)
			{
				/* select Parents */
				sel1 = rand() % ((3+GEN_SIZE)/4);

				sel2 = rand() % ((3+GEN_SIZE)/4);

				/* Crossover */
				for (int k=0; k<vertex; ++k)
					a[k] = rand()%2;
				Solution *temp_Sol = new Solution(vertex, sol[sel1], sol[sel2], a);
				temp_Sol->getValue(&graph);
				now_sol.push_back(temp_Sol);
				/* Mutate */
				for (int k=0; k<vertex; ++k)
				{
					if (rand() % MUTATE == 0)
					{
						now_sol[j]->mutateKey(k, &graph);
						now_sol[j]->flipKey(k);
					}
				}
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

			for (int j=0; j<GEN_SIZE; ++j)
			{
				delete sol[j];
			}
			sol.clear();
			sol.assign(now_sol.begin(), now_sol.end());
#ifdef PRINT_TIME
			//						std::cout<<average/(float)GEN_SIZE << " "<<first_max <<" " <<max_new<<std::endl;
#endif
			gettimeofday(&t2, NULL);
			if (repeats == 0 && t2.tv_sec - t1.tv_sec > 90)
				i += MAX_GEN; 
			std::cout<<i<<" / "<<MAX_GEN<<" : "<<(t2.tv_sec - t1.tv_sec) <<" secs"<<std::endl;
		}

		struct timeval tt1, tt2;
		long _count = 0;
		Solution *temp_sol;
		gettimeofday(&tt1, NULL);
		int max_index_s = INT_MIN;
		int max_value_s = INT_MIN;
for (int ii=0; ii<GEN_SIZE;++ii)
{
		temp_sol = sol[ii];
		int check_total = vertex + edge;
		int improved = check_total;
		int j=0;
		while (improved)
		{
			j = (j+1) % check_total;
			if (j < vertex && temp_sol->mutateKey(sigma[j], &graph) > 0)
			{
				temp_sol->flipKey(sigma[j]);
				improved = check_total;
			}
			else if (j >= vertex && temp_sol->mutateEdge(edgeSigma[j-vertex], &graph) > 0)
			{
				temp_sol->flipEdge(edgeSigma[j-vertex], &graph);
				improved = check_total;
			}
			else
			{
				--improved;
			}
		}
		if (max_value_s < temp_sol->getValue(&graph))
		{
			max_value_s = temp_sol->getValue(&graph);
			max_index_s = ii;
		}

}
		gettimeofday(&tt2, NULL);
		std::cout<<"Local Spent: "<<((tt2.tv_sec-tt1.tv_sec)*1000 + (tt2.tv_usec-tt1.tv_usec)*0.001f) <<" msecs "<<std::endl; //_count<<", avg: "<<total_sum/GEN_SIZE<<std::endl;

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
		if ((t2.tv_sec - t1.tv_sec) *(repeats+1) > 150 * repeats)
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