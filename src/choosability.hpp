/**
 * choosability.hpp
 *
 * Method declarations for determining if an object is choosable.
 */

#ifndef CHOOSABILITY_HPP__
#define CHOOSABILITY_HPP__

#include "nausparse.h"

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
bool isLChoosable(sparsegraph* g, int r, bool* L, int* Lstart, int* Lsizes, bool print=false);

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
bool isKChoosable(sparsegraph* g, int k, int separation=-1, int union_size=-1, bool print=false);

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
bool isFChoosable(sparsegraph* g, int* f, int separation=-1, int union_size=-1, bool print=false);

/**
 * isKReducible
 *
 * Given a configuration (G, X, ext) where G is a graph, 
 * X is a set of vertices in G, and ext is an external degree function,
 * create the function f such that we care about G being f-choosable
 * (or (f,c)-choosable if separation = c > 0).
 */
bool isKReducible(int k, sparsegraph* g, bool* X, int* ext, int separation=-1, int union_size=-1, bool print=false);

#endif
