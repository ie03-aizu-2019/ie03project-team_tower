#include <stdio.h>
#include "point.h"
#include "input.h"
#include "test.h"

#define NMAX 1000
#define MMAX 500
#define CROSS 1000
#define QMAX 100

int main() {
  int n, m, p, q;

  point_t point[NMAX+1];
  
  /* 入力部分 */
  inputNumber(&n, &m, &p, &q);
  inputPoint(point, n);
  inputRoad(point, road, m);

  /* テスト出力 */
  outputPoint(point, n);
  outputRoad(road, m);
  
  return 0;
}

