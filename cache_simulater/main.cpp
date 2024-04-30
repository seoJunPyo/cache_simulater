#include "Cache.h"

CacheSimulator cs;

struct A
{
	alignas(64)
	int b = 0;
};

void AAA(int n)
{
	if (n == 0)
		return;

	int a = 0;
	cs.CacheHit(&a);

	int b = 0; 
	cs.CacheHit(&b);

	AAA(n - 1);
}

int main()
{
	AAA(100);

	return 0;
}