#include <stdio.h>
#include <stdlib.h>
#include "point.h"
#include "input.h"
#include "func.h"
#include "func_shortest.h"

#define NMAX 1000
#define MMAX 500
#define CROSS 499500
#define QMAX 100
#define PQ 2

int main() {
  int n, m, p, q;
  int i, j;
  int indexFori = 0;
  int crossIndex = 0;
  int crossCount = 0;   // 交点の総数
  
  int startid[QMAX], goalid[QMAX];
  int numberOfPoint;
  double shortestDistance;

  int** road;  // 0: 端点Pのid, 1: 端点Qのid
  int roadA_P, roadA_Q, roadB_P, roadB_Q;  // 道Aの端点Pのid, 道Aの端点Qのid, 道Bも同様

  point_t* point;
  point_t* crossing;
  point_t tmpPoint;

  double** edge;  // 辺: 中身は座標間の距離

  int* low;
  int* pre;
  
  /* 入力部分 */
  inputNumber(&n, &m, &p, &q);

  point = (point_t*) malloc(sizeof(point_t) * (NMAX + CROSS));
  road = (int**) malloc(sizeof(int*) * m);
  road[0] = (int*) malloc(sizeof(int) * m * PQ);
  for(i = 1; i < m; i++) {
    road[i] = road[i - 1] + PQ;
  }
  edge = (double**) malloc(sizeof(double*) * (NMAX + CROSS));
  edge[0] = (double*) malloc(sizeof(double) * (NMAX + CROSS) * (NMAX + CROSS));
  for(i = 1; i < (NMAX + CROSS); i++) {
    edge[i] = edge[i - 1] + (NMAX + CROSS);
  }
  
  crossing = (point_t*) malloc(sizeof(point_t) * CROSS);
  
  inputPoint(point, n);
  inputRoad(point, road, m);
  inputPath(startid, goalid, q, n);

  /* 交差地点を探し出す部分 */
  for(i = 0; i < m - 1; i++) {
    for(j = i + 1; j < m; j++) {
      roadA_P = road[i][0];
      roadA_Q = road[i][1];
      roadB_P = road[j][0];
      roadB_Q = road[j][1];

      tmpPoint =
	detectCrossing(point[roadA_P - 1], point[roadA_Q - 1], point[roadB_P - 1], point[roadB_Q - 1]);
      if( (tmpPoint.x != -1) && (tmpPoint.y != -1) ) {
	crossing[crossCount] = tmpPoint;
	crossing[crossCount].roadA = i + 1;    // 交差地点は道iの上にある
	crossing[crossCount].roadB = j + 1;    // 交差地点は道jの上にある
	crossCount++;
      }
    }
  }

  /* xが小さい順にソート */
  sortCrossing(crossing, crossCount, n);

  // 交差地点を座標の構造体配列へ格納
  crossIndex = n;
  for(i = 0; i < crossCount; i++) {
    point[crossIndex] = crossing[i];
    crossIndex++;
  }
  free(crossing);
  numberOfPoint = n + crossCount;  // 全ての座標の数

  // 全ての座標を出力（交差地点を含める）
  printf("~ point(including crossing point) ~\n");
  for(i = 0; i < numberOfPoint; i++) {
    printf("%d id: %d, x:%lf, y:%lf, roadA:%d, roadB:%d\n", i, point[i].id, point[i].x,
	   point[i].y, point[i].roadA, point[i].roadB);
  }

  // 全ての道を出力
  printf("~ road list ~\n");
  for(i = 0; i < m; i++) {
    printf("%d P:%d, Q:%d\n", i+1, road[i][0], road[i][1]);
  }

  // 辺をつくる
  generateEdge(edge, point, road, m, n, crossCount);

  printf("~ bridge ~\n");
  // 橋の検出
  // 初期化
  low = (int*) malloc(numberOfPoint * sizeof(int));
  pre = (int*) malloc(numberOfPoint * sizeof(int));
  for(i = 0; i < numberOfPoint; i++) {
    low[i] = -1;
    pre[i] = -1;
  }

  for(i = 1; i <= numberOfPoint; i++) {
    indexFori = searchPointIndex(point, numberOfPoint, i);
    // 全てのidについてやったらbreak
    if(indexFori == -1) break;
    bridgeDfs(point, edge, indexFori, indexFori, low, pre, numberOfPoint);
  }

  return 0;
}
