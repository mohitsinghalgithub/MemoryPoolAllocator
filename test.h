#pragma once 
#include <iostream>
#include <vector>
#include <thread>
#include <cassert>
#include <chrono>
#include <set>

#include "FixedPool.h"
#include "VariableMemoryPool.h"

using namespace std;

#define EXTPECT_TRUE(x) assert((x))
#define EXTPECT_EQ(x, y) assert((x == y))

class VariableMemoryPoolTest
{
public:
	void basicTest();
	void ThreadTest();
};

class FixedPoolTest
{
public:
	void basicTest();
	void ThreadTest();
};
