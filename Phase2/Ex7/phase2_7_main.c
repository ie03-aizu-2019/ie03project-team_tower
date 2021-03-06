#include <stdio.h>
#include <stdlib.h>
#include "point.h"
#include "input.h"
#include "func_newRoad.h"
#include "test.h"

#define NMAX 1000
#define MMAX 500
#define CROSS 499500
#define QMAX 100
#define PMAX 100
#define PQ 2

int main() {
  int n, m, p, q;
  int i;

  point_t* point;
  point_t* newPoint;
  point_t suggestPoint;
  int** road;
  
  /* 入力部分 */
  inputNumber(&n, &m, &p, &q);

  point = (point_t*) malloc(sizeof(point_t) * (NMAX + CROSS));
  newPoint = (point_t*) malloc(sizeof(point_t) * PMAX);
  road = (int**) malloc(sizeof(int*) * m);
  road[0] = (int*) malloc(sizeof(int) * m * PQ);
  for(i = 1; i < m; i++) {
    road[i] = road[i - 1] + PQ;
  }
  
  inputPoint(point, n);
  inputRoad(point, road, m);
  inputNewPoint(newPoint, p);

  // 最適な道の建設提案
  for(i = 0; i < p; i++) {
    suggestPoint = suggestNewRoad(point, road, n, m, newPoint[i]);
    printf("%f %f\n", suggestPoint.x, suggestPoint.y);
  }
  
  return 0;
}

