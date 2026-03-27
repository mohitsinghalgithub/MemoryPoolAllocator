#include <iostream>
#include <vector>
#include <thread>
#include <cassert>
#include <chrono>
#include <set>
#include "test.h"


void VariableMemoryPoolTest::basicTest()
{
	VariableMemoryPool pool;

	int * ptr = static_cast<int*> (pool.allocate(sizeof(int)));
	*ptr = 10;
	EXTPECT_EQ(*ptr, 10);
	pool.deallocate(ptr, sizeof(int));
	
	

}


