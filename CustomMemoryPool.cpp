#include "FixedPool.h"
#include "VariableMemoryPool.h"

FixedPool::FixedPool(size_t size, size_t count)
{
	blockSize = max (size, sizeof(Block));
	memory = ::operator new(blockSize*count);

	char *data = static_cast<char *>(memory);
	for(size_t i = 0; i < count; i++)
	{
		Block *blockData = reinterpret_cast<Block*>(data + i * blockSize);
		blockData->next = freeList;
		freeList = blockData;
	}
}

void * FixedPool::allocate()
{
	unique_lock<mutex> lock(mxt);
	
	bool condition_met = cond.wait_for(lock, std::chrono::seconds(1),[this](){return freeList != nullptr;});
	if(!condition_met)
		throw bad_alloc();
	Block *data = freeList;
	freeList = freeList->next;
	lock.unlock();
	return data;
}

void FixedPool::deallocate(void *ptr)
{
	if(!ptr)
		return;
	Block * data = reinterpret_cast<Block*>(ptr);
	unique_lock<mutex> lock(mxt);
	data->next = freeList;
	freeList = data;
	lock.unlock();
	cond.notify_one();
	ptr = nullptr;
}

VariableMemoryPool::VariableMemoryPool()
{
	for(const auto &size : sizes)
	{
		
		pool.emplace_back(make_unique<FixedPool>(size, 1024));
	}
}

int VariableMemoryPool::getIndex(const size_t size)
{
	for(int i = 0; i < NUM_CLASSES; i++)
	{
		if(size <= sizes[i])
			return i;
	}
	throw std::bad_alloc();
}

void *VariableMemoryPool::allocate(size_t size)
{
	return pool[getIndex(size)]->allocate();
}

void VariableMemoryPool::deallocate(void * ptr, size_t size)
{
	pool[getIndex(size)]->deallocate(ptr);
}


template <typename T>
class PoolAllocator
{
	VariableMemoryPool *pool;

public:
	using value_type = T;
	PoolAllocator(VariableMemoryPool *p = nullptr):pool(p){}

	template <class U>
	PoolAllocator (const PoolAllocator<U> & other): pool(other.pool){}

	T* allocate(size_t n)
	{
		return static_cast<T*> (pool->allocate(n * sizeof(T)));
	}

	void deallocate(value_type *ptr, size_t size)
	{
		pool->deallocate(ptr,  size * sizeof(T));
	}

	template <class U>
    bool operator==(const PoolAllocator<U>& other) const {
        return pool == other.pool;
    }

    template <class U>
    bool operator!=(const PoolAllocator<U>& other) const {
        return !(*this == other);
    }
};

// int main()
// {
// 	VariableMemoryPool pool;
	
//     std::vector<int, PoolAllocator<int>> vec{PoolAllocator<int>(&pool)};

//     for (int i = 0; i < 10; i++)
//         vec.push_back(i);

//     for (int x : vec)
//         std::cout << x << " ";

// 	return 0;
// }