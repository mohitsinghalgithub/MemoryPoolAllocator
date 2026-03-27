#pragma once 

#include <iostream>
#include <mutex>
#include <atomic> 
#include <vector>
#include <condition_variable>

using namespace std;

class FixedPool
{
	struct Block
	{
		Block *next;
	};

	Block * freeList = nullptr;
	void *memory;
	size_t blockSize;
	mutex mxt;
	condition_variable cond;
public:
	FixedPool(size_t size, size_t count);

	void *allocate();
	
	void deallocate(void *ptr);

	~FixedPool()
	{
		::operator delete (memory);
	}

	FixedPool(const FixedPool &) = delete;
	FixedPool & operator= (const FixedPool &) = delete;
};

