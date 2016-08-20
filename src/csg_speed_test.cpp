/**
 * csg_test.cpp
 *
 * an executable to test the connected subgraph enumeration.
 */

#include "nausparse.h"
#include "gtools.h"
#include "csg_enumeration.hpp"
#include <time.h>


int main(int argc, char** argv)
{
	sparsegraph gobj;
	SG_INIT(gobj);

	sparsegraph* g = &gobj;

	int num_loops = 0;
	stringtosparsegraph(argv[1], g, &num_loops);

	double t1, t2;

	bool* active = (bool*)malloc(g->nv);
	bool* s = (bool*)malloc(g->nv);
	int size = 0;

	for ( int i = 0;i < g->nv; i++ )
	{
		active[i] = true;
	}

	{
		clock_t start_clock = clock();
		int count = 0;
		initCSPrimary(g,active,s,size);
		do
		{
			count++;
		} while ( nextCSPrimary(g,active,s,size)  );
		clock_t end_clock = clock();

		t1 = (end_clock-start_clock)/(double)CLOCKS_PER_SEC;
	}


	{
		clock_t start_clock = clock();
		int count = 0;
		initCSReversePrimary(g,active,s,size);
		do
		{
			count++;
		} while ( nextCSReversePrimary(g,active,s,size)  );
		clock_t end_clock = clock();

		t2 = (end_clock-start_clock)/(double)CLOCKS_PER_SEC;
	}


	printf("Time for Primary Order: %3.8lf seconds.    ", t1);
	printf("Time for Reverse Order: %3.8lf seconds.    ", t2);
	printf("t1/t2 = %4.3lf\n", t1/t2);

	return 0;
}