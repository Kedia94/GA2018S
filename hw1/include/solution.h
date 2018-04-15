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

public:
	int _value;
	Solution(int size, int *array);
	Solution(int size, Solution* sol1, Solution* sol2, int *randarray);
	~Solution();
	int isInclude(int key);
	void mutateKey(int key);
	int getValue(Graph graph);
	int getValue();
	void print();

	bool operator<(const Solution &a) const {
			std::cout<<_value<<" < "<<a._value<<std::endl;
			return _value < a._value;
	}
	
};

#endif
