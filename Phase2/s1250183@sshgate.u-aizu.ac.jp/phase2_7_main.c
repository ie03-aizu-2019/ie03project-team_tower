#include <stdio.h>
#include "point.h"
#include "input.h"
#include "func_newRoad.h"
#include "test.h"

#define NMAX 1000
#define MMAX 500
#define CROSS 1000
#define QMAX 100
#define PMAX 100

int main() {
  int n, m, p, q;
  int i;

  point_t point[NMAX+1];
  point_t newPoint[PMAX];
  point_t suggestPoint;
  int road[MMAX+1][2];
  
  /* 入力部分 */
  inputNumber(&n, &m, &p, &q);
  inputPoint(point, n);
  inputRoad(point, road, m);
  inputNewPoint(newPoint, p);

  // 最適な道の建設提案
  for(i = 0; i < p; i++) {
    suggestPoint = suggestNewRoad(point, road, n, m, newPoint[i]);
    printf("%f %f\n", suggestPoint.x, suggestPoint.y);
  }

  /* テスト出力 */
  outputPoint(point, n);
  outputRoad(road, m);
  outputNewPoint(newPoint, p);
  
  return 0;
}

