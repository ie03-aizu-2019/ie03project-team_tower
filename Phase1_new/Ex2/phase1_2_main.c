/*  s1250183 Yuki Homma  */
/* new version -- malloc */
/*  ----- constraint ------- */
/* | 2<=N<=200, 1<=M<=100   |*/
/* | 0<=x,y<=1000, P=0, Q=0 |*/
/* ------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include "func.h"
#include "input.h"
#include "point.h"
#define PQ 2
#define NMAX 200
#define MMAX 100
#define CROSS 5000

int main() {
  int n, m, p, q;
  int idP, idQ;
  int i, j;
  int crossIndex = 0;
  int crossCount = 0;

  int** road;
  int roadA_P, roadA_Q, roadB_P, roadB_Q;  // 道Aの端点Pのid, 道Aの端点Qのid, 道Bも同様

  point_t* point;
  point_t* crossing;
  point_t tmpPoint;

  inputNumber(&n, &m, &p, &q);

  point = (point_t*) malloc(sizeof(point_t) * NMAX);
  road = (int**) malloc(sizeof(int*) * m);
  road[0] = (int*) malloc(sizeof(int) * m * PQ);
  for(i = 1; i < m; i++) {
    road[i] = road[i - 1] + PQ;
  }
  crossing = (point_t*) malloc(sizeof(point_t) * CROSS);

  inputPoint(point, n);
  inputRoad(point, road, n, m);

  // test output
  for(i = 0; i < n; i++) {
    printf("%d id:%d x:%lf y:%lf roadA:%d roadB%d\n", i, point[i].id, point[i].x, point[i].y,
	   point[i].roadA, point[i].roadB);
  }

  for(i = 0; i < m; i++) {
    printf("%d P:%d, Q:%d\n", i, road[i][0], road[i][1]);
  }

  /* 交差地点を探し出す部分 */
  for(i = 0; i < m - 1; i++) {
    for(j = i + 1; j < m; j++) {
      roadA_P = road[i][0];
      roadA_Q = road[i][1];
      roadB_P = road[j][0];
      roadB_Q = road[j][1];

      tmpPoint =
      	detectCrossing(point[roadA_P], point[roadA_Q], point[roadB_P], point[roadB_Q]);
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
  
  for(i = 0; i < crossIndex; i++) {
    printf("point:%d x:%f, y:%f, roadA:%d, roadB:%d\n", point[i].id, point[i].x, point[i].y,
	   point[i].roadA, point[i].roadB);
  }

  free(point);
  free(road);

  return 0;
}

