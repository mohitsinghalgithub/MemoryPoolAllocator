#include "test.h"



void FixedPoolTest::basicTest()
{
	FixedPool pool(8, 3);

	int * ptr = static_cast<int *> (pool.allocate());
	EXTPECT_TRUE(ptr != nullptr);

	*ptr = 10;

	EXTPECT_EQ(*ptr, 10);
	*ptr = 11;
	EXTPECT_EQ(*ptr, 11);

	pool.deallocate(ptr);
	
	int * ptr2 = static_cast<int *> (pool.allocate());
	*ptr2 = 'c';
	EXTPECT_EQ(*ptr2, 'c');
	EXTPECT_TRUE(ptr == ptr2);
	pool.deallocate(ptr2);

	char * ptr1 = static_cast<char *> (pool.allocate());
	*ptr1 = 'c';
	EXTPECT_EQ(*ptr1, 'c');
	

	pool.deallocate(ptr1);

	set<int*> pointerSet;
	for(int i = 0; i < 3; i++)
	{
		pointerSet.emplace(static_cast<int*> (pool.allocate()));
	}

	EXTPECT_TRUE(pointerSet.size() == 3);
	for(auto itr : pointerSet)
	{
		pool.deallocate(itr);
	}

}

void FixedPoolTest::ThreadTest()
{
	FixedPool pool(8, 5);

	auto worker = [](FixedPool *pool)
	{
		for(int i = 0; i < 5;i++)
		{
			int *ptr  = static_cast<int*> (pool->allocate());
			*ptr = i;
			EXTPECT_EQ(*ptr, i);
			
			pool->deallocate(ptr);
			ptr = nullptr;
		}
	};

	thread th1(worker, &pool);
	thread th2(worker, &pool);
	th1.join();
	th2.join();
}
