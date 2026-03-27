#include "test.h"

int main()
{
	FixedPoolTest testPoolObj;

	testPoolObj.basicTest();
	testPoolObj.ThreadTest();

	VariableMemoryPoolTest vMPool;

	vMPool.basicTest();
	return 0;
}