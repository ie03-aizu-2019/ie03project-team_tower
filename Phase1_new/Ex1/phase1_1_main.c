/* new version */
#include <stdio.h>
#include "func.h"
#include "input.h"
#include "point.h"

#define NMAX 4
#define MMAX 2

int main() {
  int n, m, p, q;
  int idP, idQ;
  int i, j;

  int road[MMAX+1][2];  // 0: 端点Pのid, 1: 端点Qのid
  int roadA_P, roadA_Q, roadB_P, roadB_Q;  // 道Aの端点Pのid, 道Aの端点Qのid, 道Bも同様

  point_t point[NMAX+1];
  point_t tmpPoint;

  /* 入力部分 */
  inputNumber(&n, &m, &p, &q);
  inputPoint(point, n);
  inputRoad(point, road, m);

  roadA_P = road[1][0];
  roadA_Q = road[1][1];
  roadB_P = road[2][0];
  roadB_Q = road[2][1];
  
  tmpPoint =
    detectCrossing(point[roadA_P], point[roadA_Q], point[roadB_P], point[roadB_Q]);
      
  if( (tmpPoint.x != -1) && (tmpPoint.y != -1) ) {
    tmpPoint.roadA = 1;
    tmpPoint.roadB = 2;
    printf("%f %f\n", tmpPoint.x, tmpPoint.y);
  } else {
    printf("NA\n");
  }
  
  return 0;
}
