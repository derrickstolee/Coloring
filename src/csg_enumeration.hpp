/**
 * csg_enumeration.hpp
 *
 * Procedures for enumerating connected (induced) subgraphs of a given graph.
 *
 * Uses the nauty sparsegraph format.
 */


#ifndef CSG_ENUMERATION_HPP__
#define CSG_ENUMERATION_HPP__

#include "nausparse.h"

/**
 * Initialize to the first active subgraph in the primary order.
 *
 * Will modify the array s and updates the given size.
 */
void initCSPrimary(sparsegraph* g, bool* active, bool* s, int& size );

/**
 * Given a connected subgraph of g, modify the set s to be the next
 * connected subgraph of the active vertices in the primary order.
 *
 * Will modify the array s and updates the given size.
 *
 * returns true if success, false if there is no next connected subgraph.
 */
bool nextCSPrimary(sparsegraph* g, bool* active, bool* s, int& size );

/**
 * Initialize to the first subgraph in the reverse primary order.
 */
void initCSReversePrimary(sparsegraph* g, bool* active, bool* s, int& size );

/**
 * Given a connected subgraph of g, modify the set s to be the next
 * connected subgraph of the active vertices in the reverse primary order.
 *
 * Will modify the array s and updates the given size.
 *
 * returns true if success, false if there is no next connected subgraph.
 */
bool nextCSReversePrimary(sparsegraph* g, bool* active, bool* s, int& size );

/**
 * Returns the canonical deletion of s
 */
int getCanonicalDeletion(sparsegraph* g, bool* active, bool* s, int size);

void printSet(bool* s, int n);

#endif
