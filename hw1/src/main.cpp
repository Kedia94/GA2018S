#include <iostream>
#include <fstream>
#include <string>

#include "graph.h"

#define PRINT_TIME

#ifdef PRINT_TIME
#include <sys/time.h>
#endif



int main()
{
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

	std::cout <<"Vertex: "<<vertex<<", Edge: "<<edge<<std::endl;
	Graph graph(vertex);
	std::cout << graph.GetNumVertex() << std::endl;

	/* Add Graph Edge */
	while (edge_iter < edge)
	{
		int v1, v2, weight;
		maxin >> v1;
		maxin >> v2;
		maxin >> weight;

		graph.AddEdge(v1, v2, weight);
//		std::cout << v1 << ", "<<v2 << ", "<< weight << std::endl;
//		std::cout << graph.GetWeight(v2, v1) << std::endl;

		edge_iter++;
	}

#ifdef PRINT_TIME
	gettimeofday(&t2, NULL);
	std::cout<<"Spent: "<<((t2.tv_sec-t1.tv_sec)*1000 + (t2.tv_usec-t1.tv_usec)*0.001f) <<" msecs"<<std::endl;
#endif
    return 0;
}
