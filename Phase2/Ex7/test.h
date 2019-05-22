#include "point.h"
#define NMAX 1000
#ifndef TEST_H
#define TEST_H

extern void outputPoint(point_t*, int);
extern void outputRoad(int (*)[2], int);
extern void outputCrossing(point_t*, int);
extern void outputEdge(double (*)[], point_t*, int (*)[2], int, int, int);

#endif

  
