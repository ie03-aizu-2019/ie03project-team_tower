#include <stdbool.h>
#include "point.h"
#include "pass.h"
/*
#define NMAX 1000
#define CROSS 1000
*/
#define NMAX 100
#define CROSS 100

#ifndef FUNC_SHORT_H
#define FUNC_SHORT_H

extern void generateEdge(double (*)[NMAX], point_t*,
			 int (*)[2], int, int, int);
extern double calcDistance(point_t, point_t);
extern double searchShortestPath(point_t*, double (*)[NMAX], pass*, int, int, int, int);
extern int searchPointIndex(point_t*, int, int);

extern bool k_searchShortestPath(point_t*, double (*)[NMAX], int, int, int, int, int);
extern bool pass_equals(pass*, pass*);
extern bool dist_equals(double, double);

#endif
