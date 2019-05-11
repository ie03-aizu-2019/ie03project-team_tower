/* new version */
#include <stdio.h>
#include "point.h"
#include "input.h"
#include "func.h"
#include "func_shortest.h"

#define NMAX 1000
#define MMAX 500
#define CROSS 1000
#define QMAX 100

int main() {
  int n, m, p, q;
  int i, j;
  int crossIndex = 0;
  int crossCount = 0;   // 交点の総数
  
  int startid[QMAX], goalid[QMAX];
  int numberOfPoint;

  int road[MMAX+1][2];  // 0: 端点Pのid, 1: 端点Qのid
  int roadA_P, roadA_Q, roadB_P, roadB_Q;  // 道Aの端点Pのid, 道Aの端点Qのid, 道Bも同様

  point_t point[NMAX+1];
  point_t crossing[CROSS];
  point_t tmpPoint;

  double edge[NMAX][NMAX];  // 辺: 中身は座標間の距離

  /* 入力部分 */
  inputNumber(&n, &m, &p, &q);
  inputPoint(point, n);
  inputRoad(point, road, m);
  inputPath(startid, goalid, q, n);

  /* 交差地点を探し出す部分 */
  for(i = 1; i < m; i++) {
    for(j = i + 1; j <= m; j++) {
      roadA_P = road[i][0];
      roadA_Q = road[i][1];
      roadB_P = road[j][0];
      roadB_Q = road[j][1];

      tmpPoint =
	detectCrossing(point[roadA_P], point[roadA_Q], point[roadB_P], point[roadB_Q]);
      if( (tmpPoint.x != -1) && (tmpPoint.y != -1) ) {
	crossing[crossCount] = tmpPoint;
	crossing[crossCount].roadA = i;    // 交差地点は道iの上にある
	crossing[crossCount].roadB = j;    // 交差地点は道jの上にある
	crossCount++;
      }
    }
  }

  /* xが小さい順にソート */
  sortCrossing(crossing, crossCount, n);

  // 交差地点を座標の構造体配列へ格納
  crossIndex = crossing[0].id;
  for(i = 0; i < crossCount; i++) {
    point[crossIndex] = crossing[i];
    crossIndex++;
  }
  numberOfPoint = n + crossCount;  // 全ての座標の数

  // 辺をつくる
  generateEdge(edge, point, road, m, n, crossCount);

  printf("\n座標間の距離\n");
  // 辺の出力テスト
  for(i = 1; i <= numberOfPoint; i++) {
    for(j = 1; j <= numberOfPoint; j++) {
      if(edge[i][j] != 0) printf("%d %d: %f\n", i, j, edge[i][j]);
    }
  }

  /* 最短経路探索 */
  searchShortestPath(point, edge, numberOfPoint);
  
  return 0;
}

