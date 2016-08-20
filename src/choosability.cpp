/**
 * choosability.cpp
 *
 * Method definitions for determining if an object is choosable.
 */


#include "nausparse.h"
#include "choosability.hpp"
#include "csg_enumeration.hpp"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stack>
#include <queue>


std::stack<int> snapshot_Lsizes_i; // decrement
std::stack<int> snapshot_Lsizes_size; 
std::stack<int> snapshot_L_i; // index of move to FALSE
std::stack<int> snapshot_L_size;
std::stack<int> snapshot_c_i; // set to a value
std::stack<int> snapshot_c_size;

void snapshot()
{
	snapshot_Lsizes_size.push(snapshot_Lsizes_i.size());
	snapshot_L_size.push(snapshot_L_i.size());
	snapshot_c_size.push(snapshot_c_i.size());
}

void rollback(bool* L, int* Lsizes, int* c)
{
	unsigned int to_size = 0;
	if ( snapshot_Lsizes_size.size() > 0 )
	{
		to_size = snapshot_Lsizes_size.top();
		snapshot_Lsizes_size.pop();
	}

	while ( snapshot_Lsizes_i.size() > to_size )
	{
		int i = snapshot_Lsizes_i.top();
		snapshot_Lsizes_i.pop();

		Lsizes[i] = Lsizes[i] + 1;
	}

	to_size = 0;
	if ( snapshot_L_size.size() > 0 )
	{
		to_size = snapshot_L_size.top();
		snapshot_L_size.pop();
	}

	while ( snapshot_L_i.size() > to_size )
	{
		int i = snapshot_L_i.top();
		snapshot_L_i.pop();

		L[i] = true;
	}

	
	to_size = 0;
	if ( snapshot_c_size.size() > 0 )
	{
		to_size = snapshot_c_size.top();
		snapshot_c_size.pop();
	}

	while ( snapshot_c_i.size() > to_size )
	{
		int i = snapshot_c_i.top();
		snapshot_c_i.pop();

		c[i] = -1;
	}
}

// This is the recursive version of the isLChoosable method.
bool isLChoosable(sparsegraph* g, int r, bool* L, int* Lstart, int* Lsizes, bool print, int* c, int last_v)
{
	// printf("c : ");
	// for ( int i = 0; i < g->nv; i++ )
	// {
	// 	if ( c[i] >= 0 )
	// 	{
	// 		printf("%d ", c[i]);
	// 	}
	// 	else
	// 	{
	// 		printf("_ ");
	// 	}
	// }
	// printf("\nL : ");
	// for ( int i = 0; i < g->nv; i++ )
	// {
	// 	printf("%d ", Lsizes[i]);
	// }
	// printf("\n");

	if ( last_v >= 0 )
	{
		// remove this color from the domains of other vertices.
		std::queue<int> q;
		q.push(last_v);

		while ( q.size() > 0 )
		{
			int v = q.front();
			q.pop();

			for ( int i = 0; i < g->d[v]; i++ )
			{
				int u = g->e[g->v[v] + i];

				if ( c[u] == c[v] )
				{
					// adjacent of same color!
					return false;
				}

				if ( L[ Lstart[u] + c[v] ] == true )
				{
					// remove the color!
					L[ Lstart[u] + c[v] ] = false;
					snapshot_L_i.push( Lstart[u] + c[v] );

					Lsizes[u] = Lsizes[u] - 1;
					snapshot_Lsizes_i.push(u);

					if ( Lsizes[u] <= 0 )
					{
						return false;
					}	
					else if ( Lsizes[u] == 1 )
 					{
 						for ( int a = 0; a < r; a++ )
 						{
 							if ( L[ Lstart[u] + a ] )
 							{
 								if ( c[u] < 0 )
 								{
	 								c[u] = a;
	 								snapshot_c_i.push(u);

	 								q.push(u);
	 							}
	 							else if ( c[u] != a )
	 							{
	 								return false;
	 							}
 							}
 						}
					}
				}
			}
		}
	} // end propagation step

	int v = -1;

	for ( int i = 0; i < g->nv; i++ )
	{
		// TODO: use a better choice here!
		if ( c[i] < 0 )
		{
			v = i;
			break;
		}
	}

	if ( v < 0 )
	{
		// Success!
		if ( print )
		{
			printf("Coloring found: \n");
			for ( int i = 0; i < g->nv; i++ )
			{
				printf("%2d : %2d", i, c[i]);

				if ( (i % 5) == 0 || i == g->nv - 1)
				{
					printf("\n");
				}
				else
				{
					printf(" | ");
				}
			}

			printf("\n");
		}

		return true;
	}

	for ( int a = 0; a < r; a++ )
	{
		if ( L[ Lstart[v] + a ] )
		{
			// we can try this color here!
			snapshot();

			for ( int aa = 0; aa < r; aa++ )
			{
				if ( aa != a && L[ Lstart[v] + aa ]) 
				{
					L[ Lstart[v] + aa ] = false;
					snapshot_L_i.push( Lstart[v] + aa );

					Lsizes[v] = Lsizes[v] - 1;
					snapshot_Lsizes_i.push(v);
				}
			}

			c[v] = a;
			snapshot_c_i.push(v);

			bool result = isLChoosable(g, r, L, Lstart, Lsizes, print, c, v);

			rollback(L, Lsizes, c);

			if ( result )
			{
				return true;
			}
		}
	}

	return false;
}

/**
 * isLChoosable
 *
 * Given an array of list assignments, can we find an L-coloring?
 *
 * if print == true, then it will print an L-coloring before returning true.
 *
 * Note: L is an array of booleans.
 * r is the total number of colors, and each vertex v
 * has at least r reserved spots in L
 * starting at Lstart[v].
 *
 * Lsizes[v] contains the number of true values in L[Lstart[v]], ..., L[Lstart[v]+r-1].
 *
 * The array L will be used to store domains, so it may change during computation
 * but will return to this state before it finishes!
 */
bool isLChoosable(sparsegraph* g, int r, bool* L, int* Lstart, int* Lsizes, bool print)
{
	for ( int i = 0; i < g->nv; i++ )
	{
		if ( Lsizes[i] == 0 )
		{
			return false;
		}
	}

	int* c = (int*)malloc(g->nv * sizeof(int));

	for ( int i = 0; i < g->nv; i++ )
	{
		c[i] = -1;
	}

	bool result = isLChoosable(g, r, L, Lstart, Lsizes, print, c, -1);

	free(c);

	return result;
}

/**
 * isKChoosable
 *
 * Given a graph and an integer k, determine if g is k-choosable.
 *
 * Will print a list assignment before returning false.
 *
 * If separation = c > 0, then it will check only (k,c)-list assignments
 * (those where adjacent lists have at most c things in common).
 */
bool isKChoosable(sparsegraph* g, int k, int separation, int union_size, bool print)
{
	int* f = (int*)malloc(g->nv * sizeof(int));

	for ( int i = 0; i < g->nv; i++ )
	{
		f[i] = k;
	}

	bool result = isFChoosable(g, f, separation, union_size, print);

	free(f);

	return result;
}



/**
 * isKReducible
 *
 * Given a configuration (G, X, ext) where G is a graph, 
 * X is a set of vertices in G, and ext is an external degree function,
 * create the function f such that we care about G being f-choosable
 * (or (f,c)-choosable if separation = c > 0).
 */
bool isKReducible(int k, sparsegraph* g, bool* X, int* ext, int separation, int union_size, bool print)
{
	int* f = (int*)malloc(g->nv * sizeof(int));

	for ( int i = 0; i < g->nv; i++ )
	{
		if ( X[i] )
		{
			f[i] = k - ext[i];
		}
		else
		{
			f[i] = 1;
		}
	}

	bool result = isFChoosable(g, f, separation, union_size, print);

	free(f);

	return result;
}


int total_ga_calls;
bool generateAssignments(sparsegraph* g, int r, int a, int* f, int separation, int union_size, bool* active, bool* cur_csg, int size, bool* L, int* Lstart, int* Lsize, bool print);

/**
 * isFChoosable
 *
 * Given a graph G and an integer function f on V(G), 
 * determine if G is f-choosable (that is, if |L(v)| >= f(v), then
 * G is L-choosable.)
 *
 * Will print a list assignment before returning false.
 *
 * NOTE: if two adjacent vertices have f(v) = f(u) = 1, then these will
 * receive distinct lists (due to thinking of them as precolored vertices).
 *
 * If separation = c > 0, then it will check only (f,c)-list assignments
 * (those where adjacent lists have at most c things in common).
 */
bool isFChoosable(sparsegraph* g, int* f, int separation, int union_size, bool print)
{
	int fsum = 0;

	for ( int i = 0; i < g->nv; i++ )
	{
		fsum += f[i];
	}

	bool* active = (bool*)malloc(g->nv * fsum * sizeof(bool));
	bool* cur_csg = (bool*)malloc(g->nv * fsum * sizeof(bool));
	bool* L = (bool*)malloc(g->nv * fsum * sizeof(bool));
	int* Lstart = (int*)malloc(g->nv * sizeof(int));
	int* Lsize = (int*)malloc(g->nv * sizeof(int));

	for ( int i = 0; i < g->nv; i++ )
	{
		Lsize[i] = 0;
		Lstart[i] = fsum * i;
	}

	for ( int i = 0; i < g->nv * fsum; i++ )
	{
		active[i] = true;
		cur_csg[i] = false;
		L[i] = false;
	}


	int size = 0;

	initCSReversePrimary(g, active, cur_csg, size);

	total_ga_calls = 0;
	bool result = generateAssignments(g, fsum, 0, f, separation, union_size, active, cur_csg, size, L, Lstart, Lsize, print);
	printf("recursive calls: %d\n", total_ga_calls);

	free(L);
	free(Lstart);
	free(Lsize);
	free(cur_csg);
	free(active);

	return result;
}



std::stack<int> snapshot_GA_Lsizes_i; // increment
std::stack<int> snapshot_GA_Lsizes_size; 
std::stack<int> snapshot_GA_L_i; // index of move to TRUE
std::stack<int> snapshot_GA_L_size;

void snapshotGA()
{
	snapshot_GA_Lsizes_size.push(snapshot_GA_Lsizes_i.size());
	snapshot_GA_L_size.push(snapshot_GA_L_i.size());
}

void rollbackGA(bool* L, int* Lsizes)
{
	unsigned int to_size = 0;
	if ( snapshot_GA_Lsizes_size.size() > 0 )
	{
		to_size = snapshot_GA_Lsizes_size.top();
		snapshot_GA_Lsizes_size.pop();
	}

	while ( snapshot_GA_Lsizes_i.size() > to_size )
	{
		int i = snapshot_GA_Lsizes_i.top();
		snapshot_GA_Lsizes_i.pop();

		Lsizes[i] = Lsizes[i] - 1;
	}

	to_size = 0;
	if ( snapshot_GA_L_size.size() > 0 )
	{
		to_size = snapshot_GA_L_size.top();
		snapshot_GA_L_size.pop();
	}

	while ( snapshot_GA_L_i.size() > to_size )
	{
		int i = snapshot_GA_L_i.top();
		snapshot_GA_L_i.pop();

		L[i] = false;
	}
}

/**
 * Recursive algorithm for computing list assignments.
 */
bool generateAssignments(sparsegraph* g, int r, int a, int* f, int separation, int union_size, bool* active, bool* cur_csg, int size, bool* L, int* Lstart, int* Lsize, bool print)
{
	total_ga_calls++;

	if ( a > 0 )
	{
		// among all active vertices, test if they should not be active any more!
		for ( int i = 0; i < g->nv; i++ )
		{
			if ( active[i] && Lsize[i] >= f[i] )
			{
				// make inactive due to a full list!
				active[i] = false;		
			}
		}

		if ( separation > 0)
		{	
			// test for latest set of things causing a collision in the separation!
			for ( int i = 0; i < g->nv; i++ )
			{
				if ( cur_csg[i] == true )
				{
					for ( int j = 0; j < g->d[i]; j++ )
					{
						int u = g->e[g->v[i] + j];

						if ( cur_csg[u] == true)
						{
							// possible that separation was destroyed!
							int count_common = 0;
							for ( int aa = 0; aa < a; aa++ )
							{
								if ( L[ Lstart[i] + aa ] && L[ Lstart[u] + aa ] )
								{
									count_common++;

									if ( count_common > separation )
									{
										// Violated commonality!
										return true;
									}
								}
							}
						}
					}
				}
			}
		}

		if ( union_size > 0)
		{	
			// test for latest set of things causing a collision in the separation!
			for ( int i = 0; i < g->nv; i++ )
			{
				if ( cur_csg[i] == true && Lsize[i] == f[i] )
				{	
					// ok, this list is complete!

					for ( int j = 0; j < g->d[i]; j++ )
					{
						int u = g->e[g->v[i] + j];

						if ( Lsize[u] == f[u] )
						{
							// both of these lists are complete! 
							// is their union too small?
							int count_union = 0;
							for ( int aa = 0; aa < a; aa++ )
							{
								if ( L[ Lstart[i] + aa ] || L[ Lstart[u] + aa ] )
								{
									count_union++;
								}
							}

							if ( count_union < union_size )
							{
								// violated union size!
								return true;
							}
						}
					}
				}
			}
		}

		// test for inactive in the cur_csg
		for ( int i = 0; i < g->nv; i++ )
		{
			if ( active[i] == false && cur_csg[i] == true )
			{
				// move to the next one!
				// will skip to next active subset
				nextCSReversePrimary(g, active, cur_csg, size);
				break;
			}
		}

		// // Test for colorability.
		// // If we can, then stop!
		// bool result = isLChoosable(g, r, L, Lstart, Lsize);

		// if ( result )
		// {
		// 	// We can color this!
		// 	return true;
		// }

		bool has_active = false;
		for ( int i = 0; i < g->nv; i++ )
		{
			if ( active[i] )
			{
				has_active = true;
			}
		}

		if ( !has_active )
		{
			// printf("Testing list assignment:\n");

			// for ( int i = 0; i < g->nv; i++ )
			// {
			// 	printf("%2d [%d vs %d]: ", i, Lsize[i], f[i]);

			// 	for ( int aa = 0; aa < a; aa++ )
			// 	{
			// 		if ( L[ Lstart[i] + aa] )
			// 		{
			// 			printf("%2d ", aa);
			// 		}
			// 	}
			// 	printf("\n");
			// }

			// Test for colorability.
			// If we can, then stop!
			bool result = isLChoosable(g, a, L, Lstart, Lsize);

			if ( result )
			{
				// We can color this!
				return true;
			}

			// We are here and we CANNOT color with the L-lists!
			if ( print )
			{
				printf("Found hard list assignment:\n");

				for ( int i = 0; i < g->nv; i++ )
				{
					printf("%2d [%d]: ", i, Lsize[i]);

					for ( int aa = 0; aa < a; aa++ )
					{
						if ( L[ Lstart[i] + aa] )
						{
							printf("%2d ", aa);
						}
					}
					printf("\n");
				}
			}

			return false;
		}
	}

	do
	{
		if ( size <= 0 )
		{
			// if zero vertices, then skip!
			continue;
		}

		// TODO: only push to this subgraph it is CONNECTED to the previously-added list!
		// Will that help much? probably not.

		// We need to check if cur_csg contains adjacent 1-lists!
		bool adj_1lists = false;
		for ( int i = 0; !adj_1lists && i < g->nv; i++ )
		{
			if ( cur_csg[i] && (f[i] == 1) )
			{
				for ( int j = 0; !adj_1lists && j < g->d[i]; j++ )
				{
					int v = g->e[ g->v[i] + j];

					if ( cur_csg[v] && (f[v] == 1) )
					{
						adj_1lists = true;
					}
				}
			}
		}

		if ( adj_1lists )
		{
			// Do not add the same color on these 1-lists!
			continue;
		}

		// we have a connected subgraph. We will add this color to the lists!
		snapshotGA();

		// push color a to all lists!
		for ( int i = 0; i < g->nv; i++ )
		{
			if ( cur_csg[i] )
			{
				L[ Lstart[i] + a ] = true;
				snapshot_GA_L_i.push( Lstart[i] + a);

				Lsize[i] = Lsize[i] + 1;
				snapshot_GA_Lsizes_i.push(i);
			}
		}

		bool* new_active = active + g->nv;
		bool* new_csg = cur_csg + g->nv;

		for ( int i = 0; i < g->nv; i++ )
		{
			new_active[i] = active[i];
			new_csg[i] = cur_csg[i];
		}

		if ( size == 1 )
		{
			for ( int i = 0; i < g->nv; i++ )
			{
				if ( cur_csg[i] )
				{
					// make this ONE thing inactive!
					new_active[i] = false;
					break;
				}
			}
		}

		bool result = generateAssignments(g, r, a + 1, f, separation, union_size, new_active, new_csg, size, L, Lstart, Lsize, print);

		rollbackGA(L, Lsize);

		if ( !result )
		{
			return false;
		}

	} while( nextCSReversePrimary(g, active, cur_csg, size) );

	return true;
}


