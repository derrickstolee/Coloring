/**
 * csg_test.cpp
 *
 * an executable to test the connected subgraph enumeration.
 */

#include "nausparse.h"
#include "csg_enumeration.hpp"


int main(int argc, char** argv)
{
	sparsegraph gobj;
	SG_INIT(gobj);

	sparsegraph* g = &gobj;

	g->nv = 4;
	if ( argc > 1 )
	{
		g->nv = atoi(argv[1]);
	}

	// make g a squared cycle!
	g->nde = g->nv * 4;
	g->v = (size_t*)malloc(g->nv * sizeof(size_t));
	g->d = (int*)malloc(g->nv * sizeof(int));
	g->e = (int*)malloc(g->nde * sizeof(int));

	// Make a squared cycle!
	for ( int i = 0; i < g->nv; i++ )
	{
		g->v[i] = 4*i;
		g->d[i] = 4;

		g->e[4*i] = (i+1)% (g->nv);
		g->e[4*i+1] = (g->nv+i-1)% (g->nv);
		g->e[4*i+2] = (i+2)% (g->nv);
		g->e[4*i+3] = (g->nv+i-2)% (g->nv);
	}

	bool* active = (bool*)malloc(g->nv * sizeof(bool));
	bool* sp = (bool*)malloc(g->nv * sizeof(bool));
	bool* srp = (bool*)malloc(g->nv * sizeof(bool));
	int sizep = 0;
	int sizerp = 0;

	for ( int i = 0; i < g->nv; i++ )
	{
		active[i] = true;
	}
	printf("PRIMARY ORDER\n");
	initCSPrimary(g,active,sp,sizep);

	do
	{
		printf("%2d ", sizep);
		printSet(sp,g->nv);
		printf("\n");
	} while ( nextCSPrimary(g,active,sp,sizep)  );





	printf("\nPRIMARY ORDER, REVERSE PRIMARY ORDER\n");
	initCSPrimary(g,active,sp,sizep);
	initCSReversePrimary(g,active,srp,sizerp);

	do
	{
		printf("%2d ", sizep);
		printSet(sp,g->nv);
		printf("   %2d ", sizerp);
		printSet(srp, g->nv);
		printf("\n");
	} while ( nextCSPrimary(g,active,sp,sizep) && nextCSReversePrimary(g,active,srp,sizerp) );


	for ( int i = 0; i < g->nv; i++ )
	{
		active[i] = ((i % 2) == 0);
	}

	printf("\nEVEN ACTIVE\n");
	printf("PRIMARY ORDER, REVERSE PRIMARY ORDER\n");
	initCSPrimary(g,active,sp,sizep);
	initCSReversePrimary(g,active,srp,sizerp);
	do
	{
		printf("%2d ", sizep);
		printSet(sp,g->nv);
		printf("   %2d ", sizerp);
		printSet(srp, g->nv);
		printf("\n");
	} while ( nextCSPrimary(g,active,sp,sizep) && nextCSReversePrimary(g,active,srp,sizerp) );


	for ( int i = 0; i < g->nv; i++ )
	{
		active[i] = true;
	}
	printf("PRIMARY ORDER, REVERSE PRIMARY ORDER\n");
	initCSPrimary(g,active,sp,sizep);
	initCSReversePrimary(g,active,srp,sizerp);

	int count = 0;
	do
	{
		printf("%2d ", sizep);
		printSet(sp,g->nv);
		printf("   %2d ", sizerp);
		printSet(srp, g->nv);
		printf("\n");

		count++;

		if ( count == 20 )
		{
			printf("MAKING ONLY EVENS ACTIVE!\n");
			for ( int i = 0; i < g->nv; i++ )
			{
				active[i] = ((i % 2) == 0);
			}
		}
	} while ( nextCSPrimary(g,active,sp,sizep) && nextCSReversePrimary(g,active,srp,sizerp) );



	return 0;
}
