#include "point.h"
#define NMAX 1000
#define CROSS 1000

#ifndef FUNC_SHORT_H
#define FUNC_SHORT_H

extern void generateEdge(double**, point_t*,
			 int**, int, int, int);
extern double calcDistance(point_t, point_t);
extern double searchShortestPath(point_t*, double**, int, int, int);
extern int searchPointIndex(point_t*, int, int);

#endif

