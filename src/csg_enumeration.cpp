/**
 * csg_enumeration.cpp
 *
 * Procedures for enumerating connected (induced) subgraphs of a given graph.
 *
 * Uses the nauty sparsegraph format.
 */

#include <queue>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "nausparse.h"
#include "csg_enumeration.hpp"

/**
 * Initialize to the first active subgraph in the primary order.
 *
 * Will modify the array s and updates the given size.
 */
void initCSPrimary(sparsegraph* g, bool* active, bool* s, int& size )
{
	// In primary order, we consider the empty subgraph as the first "connected" subgraph.
	for ( int i = 0; i < g->nv; i++ )
	{
		s[i] = 0;
	}

	size = 0;
}

/**
 * Given a connected subgraph of g, modify the set s to be the next
 * connected subgraph of the active vertices in the primary order.
 *
 * Will modify the array s and updates the given size.
 *
 * returns true if success, false if there is no next connected subgraph.
 */
bool nextCSPrimary(sparsegraph* g, bool* active, bool* s, int& size )
{
	bool has_inactive = false;

	for ( int i = 0; !has_inactive && i < g->nv; i++ )
	{
		has_inactive = ( active[i] == false && s[i] == true );
	}

	int last_augmentation = g->nv;
	while ( has_inactive )
	{
		int v = getCanonicalDeletion(g, active, s,size);

		if ( v < 0 )
		{
			// we have deleted everything! 
			// Nowhere to go!
			// Should also not reach here, as we do NOT have any active things
			break;
		}

		s[v] = false;
		size--;

		// for knowing where to add next!
		last_augmentation = v;

		has_inactive = false;

		for ( int i = 0; !has_inactive && i < g->nv; i++ )
		{
			has_inactive = ( active[i] == false && s[i] == true );
		}
	}

	do
	{
		// Attempt an augmentation!
		// This subset is a set we have checked already, so try to augment to success!
		for ( int j = last_augmentation-1; j >= 0; j-- )
		{
			// Try adding the vertex j
			if ( s[j] == false && active[j] == true )
			{
				// Test for connectivity
				bool connected = false;
				if ( size > 0 )
				{
					for ( int k = 0; !connected && k < g->d[j]; k++ )
					{
						if ( s[ g->e[ g->v[j] + k] ] )
						{
							connected = true;
						}
					}
				}

				if ( size == 0 || connected )
				{
					// do the augmentation
					s[j] = true;
					size++;

					if ( j == getCanonicalDeletion(g,active,s,size) )
					{
						// We have successfully augmented!
						return true;
					}

					size--;
					s[j] = false;
				}
			}
		}

		// If we reached here, then none of the augmentations succeeded.
		last_augmentation = getCanonicalDeletion(g,active,s,size);

		if ( last_augmentation >= 0 )
		{
			s[last_augmentation] = false;
			size--;
		}
		else
		{
			return false;
		}
	} while(1);
}



/**
 * Initialize to the first subgraph in the reverse primary order.
 */
void initCSReversePrimary(sparsegraph* g, bool* active, bool* s, int& size )
{
	size = 0;
	for ( int i = 0;i < g->nv; i++ )
	{
		s[i] = false;
	}

	int last_augmentation = -1;
	bool has_augmentation = false;
	do
	{ 
		has_augmentation = false;
		
		// Attempt an augmentation!
		for ( int j = last_augmentation + 1; !has_augmentation && j < g->nv; j++ )
		{
			// Try adding the vertex j
			if ( s[j] == false && active[j] == true )
			{
				// Test for connectivity
				bool connected = false;
				for ( int k = 0; size > 0 && !connected && k < g->d[j]; k++ )
				{
					if ( s[g->e[g->v[j]+k]] )
					{
						connected = true;
					}
				}

				if ( size == 0 || connected )
				{
					// do the augmentation
					s[j] = true;
					size++;

					if ( j == getCanonicalDeletion(g,active,s,size) )
					{
						// We have successfully augmented!
						// Instead of returning with success, deepen and continue!
						last_augmentation = -1;
						has_augmentation = true;
					}
					else
					{
						size--;
						s[j] = false;
					}
				}
			}
		}
	} while(has_augmentation);
}

/**
 * Given a connected subgraph of g, modify the set s to be the next
 * connected subgraph of the active vertices in the reverse primary order.
 *
 * Will modify the array s and updates the given size.
 *
 * returns true if success, false if there is no next connected subgraph.
 */
bool nextCSReversePrimary(sparsegraph* g, bool* active, bool* s, int& size )
{
	// The VERY LAST one is the empty set...
	if ( size <= 0 )
	{
		return false;
	}

	// START by performing a canonical deletion!
	int last_augmentation = getCanonicalDeletion(g,active,s,size);

	s[last_augmentation] = false;
	size--;

	// Test for inactivity
	bool has_inactive = false;

	for ( int i = 0; !has_inactive && i < g->nv; i++ )
	{
		has_inactive = ( active[i] == false && s[i] == true );
	}

	// Try to augment, if success continue. Return the set that fails all further augmentations.
	while ( has_inactive )
	{
		int v = getCanonicalDeletion(g, active, s,size);

		if ( v < 0 )
		{
			// we have deleted everything! 
			// Nowhere to go!
			// Should also not reach here, as we do NOT have any active things
			break;
		}

		s[v] = false;
		size--;

		// for knowing where to add next!
		last_augmentation = v;

		has_inactive = false;

		for ( int i = 0; !has_inactive && i < g->nv; i++ )
		{
			has_inactive = ( active[i] == false && s[i] == true );
		}
	}

	bool has_augmentation = false;
	do
	{ 
		has_augmentation = false;
		
		// Attempt an augmentation!
		for ( int j = last_augmentation + 1; !has_augmentation && j < g->nv; j++ )
		{
			// Try adding the vertex j
			if ( s[j] == false && active[j] == true )
			{
				// Test for connectivity
				bool connected = false;

				if ( size > 0 )
				{
					for ( int k = 0; !connected && k < g->d[j]; k++ )
					{
						if ( s[g->e[g->v[j]+k]] )
						{
							connected = true;
						}
					}
				}

				if ( size == 0 || connected )
				{
					// do the augmentation
					s[j] = true;
					size++;

					if ( j == getCanonicalDeletion(g,active,s,size) )
					{
						// We have successfully augmented!
						// Instead of returning with success, deepen and continue!
						last_augmentation = -1;
						has_augmentation = true;
					}
					else
					{
						size--;
						s[j] = false;
					}
				}
			}
		}
	} while( has_augmentation );

	return true;
}



bool* bfs_data = 0;

/**
 * Let v be a vertex in a connected subgraph s.
 *
 * Is v the canonical deletion?
 */
int getCanonicalDeletion(sparsegraph* g, bool* active, bool* s, int size)
{
	// Return the maximum element that is NOT a cut-vertex!
	for ( int i = g->nv - 1; i >= 0; i-- )
	{
		if ( s[i] == true )
		{
			if ( size <= 2 )
			{
				return i;
			}

			// Else, test BFS
			int v = -1;
			for ( int j = 0; v < 0 && j < g->d[i]; j++ )
			{
				int u = g->e[g->v[i]+j];

				if ( s[u] == true )
				{
					v = u;
				}
			}

			if ( v < 0 )
			{
				// problem!
				printf("did not find a neighbor of %d in set.", i);
				printSet(s,g->nv);
				printf("\n");
				// Will not happen when size > 2 and s is a connected set!
				return -1;
			}

			if ( bfs_data == 0 )
			{
				bfs_data = (bool*)malloc(g->nv * sizeof(bool));
			}

			for ( int j = 0; j < g->nv; j++ )
			{
				bfs_data[j] = false;
			}

			bfs_data[v] = true;
			std::queue<int> q;
			q.push(v);
			// printSet(s, g->nv);
			// printf(" i: %d -> q: %d ", i, v);
			while ( q.size() > 0 )
			{
				v = q.front();
				q.pop();

				for ( int j = 0; j < g->d[v]; j++ )
				{
					int u = g->e[g->v[v]+j];

					if ( u != i && s[u] && bfs_data[u] == false )
					{
						bfs_data[u] = true;
						q.push(u);
						// printf(" %d ", u);
					}
				}
			}

			// printf("\n");

			// now to test the bfs_data
			bool is_cut_vertex = false;
			for ( int j = 0; !is_cut_vertex && j < g->nv; j++ )
			{
				if ( j != i && s[j] == true && bfs_data[j] == false )
				{
					is_cut_vertex = true;
				}
			}

			if ( !is_cut_vertex )
			{
				// printf("\tcanonical deletion: %d\n",i);
				return i;
			}
		}
	}

	return -1;
}



void printSet(bool* s, int n)
{
	for ( int i = 0; i < n; i++ )
	{
		if ( s[i] )
		{
			printf("+");
		}
		else
		{
			printf("_");
		}
	}
}


