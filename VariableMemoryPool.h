#pragma once 

#include "FixedPool.h"

using namespace std;

class VariableMemoryPool
{
private:
	static constexpr int NUM_CLASSES = 6;
	size_t sizes[NUM_CLASSES] = {8, 16, 32, 64, 128, 256};

	vector<FixedPool *> pool;	
public:
	VariableMemoryPool();

	~VariableMemoryPool()
	{
		for(auto & poolData : pool)
		{
			delete poolData;
		}
	}

	int getIndex(const size_t size);

	void *allocate(size_t size);

	void deallocate(void * ptr, size_t size);
};
