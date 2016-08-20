/**
 * lists.cpp
 *
 * an executable to test list-colorability of graphs and configurations
 */

#include "nausparse.h"
#include "gtools.h"
#include "csg_enumeration.hpp"
#include "choosability.hpp"
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define REDUCIBLE 1
#define CHOOSABLE 2
#define FCHOOSABLE 3

int main(int argc, char** argv)
{
	int mode = CHOOSABLE;
	int k = 4;
	int separation = -1;
	int union_size = -1;
	bool print = false;

	for ( int i = 1; i < argc; i++ )
	{
		if ( strcmp(argv[i], "--print") == 0 )
		{
			print = true;
		}
		if ( strcmp(argv[i], "--reducible") == 0 )
		{
			mode = REDUCIBLE;
		}
		if ( strcmp(argv[i], "--choosable") == 0 )
		{
			mode = CHOOSABLE;
		}
		if ( strcmp(argv[i], "--fchoosable") == 0 )
		{
			mode = FCHOOSABLE;
		}

		if ( strcmp(argv[i], "-k") == 0 && i < argc - 1)
		{
			k = atoi(argv[i+1]);
		}
		if ( strcmp(argv[i], "-c") == 0 && i < argc - 1)
		{
			separation = atoi(argv[i+1]);
		}
		if ( strcmp(argv[i], "-u") == 0 && i < argc - 1)
		{
			union_size = atoi(argv[i+1]);
		}
	}

	size_t buflen = 1000;
	char* buffer = (char*)malloc(buflen);
	
	while ( getline(&buffer, &buflen, stdin) > 1 )
	{	
		SG_DECL(gobj);
		SG_INIT(gobj);

		sparsegraph* g = &gobj;

		int num_loops = 0;
		stringtosparsegraph(buffer, g, &num_loops);
		int* f = 0;

		int count_csgs = 0;
		int size = 0;
		bool* csg = (bool*)malloc(g->nv);
		bool* active = (bool*)malloc(g->nv);
		for ( int i = 0; i < g->nv; i++ )
		{
			active[i] = true;
		}

		initCSReversePrimary(g, active, csg, size);

		do
		{
			count_csgs++;
		} while ( nextCSReversePrimary(g, active, csg, size) ); 
		printf("%d connected subgraphs\n", count_csgs);

		clock_t start_clock = clock();

		if ( mode == CHOOSABLE )
		{
			bool result = isKChoosable(g, k, separation, union_size, print);

			char* s =  sgtos6(g);
			s[strlen(s)-1] = 0;
			if ( result )
			{
				printf("%s is %d-choosable.\n", s, k);
			}
			else
			{
				printf("%s is NOT %d-choosable.\n", s, k);
			}
		}

		if ( mode == FCHOOSABLE || mode == REDUCIBLE )
		{
			// f will either be size function or ext degree
 			f = (int*)malloc(g->nv * sizeof(int));
		
			int result = getline(&buffer, &buflen, stdin);

			if ( result )
			{
				exit(result);
			}
			
 			char* tok = strtok(buffer, " ");

 			int i = 0;
 			while ( i < g->nv && tok != 0 && strlen(tok) > 0 )
 			{
 				f[i] = atoi(tok);
 				i++;

 				tok = strtok(0, " ");
 			}

 			if ( mode == FCHOOSABLE )
 			{
				bool result = isFChoosable(g, f, separation, union_size, print);

				char* s =  sgtos6(g);

				s[ strlen(s) - 1 ] = 0;
				if ( result )
				{
					printf("%s is f-choosable (with given f-values).\n",s);
				}
				else
				{
					printf("%s is NOT f-choosable (with given f-values).\n",s);
				}
 			}

 			if ( mode == REDUCIBLE )
 			{
 				// EXTRA info if external degree function is known (and will be k-1 when not in X)
 				bool* X = (bool*)malloc(g->nv * sizeof(bool));
 				for ( int i = 0; i < g->nv; i++ )
 				{
 					X[i] = true;
 				}

				bool result = isKReducible(k, g, X, f, separation, union_size, print);

				char* s =  sgtos6(g);
				s[strlen(s)-1] = 0;
				if ( result )
				{
					printf("%s is %d-reducible (with external degrees", s, k);
					for ( int i = 0;i < g->nv; i++ )
					{
						printf(" %d", f[i]);
					}
					printf(")\n");
				}
				else
				{
					printf("%s is NOT %d-reducible (with external degrees", s, k);
					for ( int i = 0;i < g->nv; i++ )
					{
						printf(" %d", f[i]);
					}
					printf(")\n");
				}

				free(X);
				X = 0;
 			}

 			free(f);
 			f = 0;
		}

		clock_t end_clock = clock();

		double t1 = (end_clock-start_clock) / (double)CLOCKS_PER_SEC;

		printf("done in %04.5lf seconds.\n\n", t1);

		SG_FREE(gobj);
	}

	return 0;
}
