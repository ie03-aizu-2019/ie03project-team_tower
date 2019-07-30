#include <stdio.h>
#include "point.h"
#define NMAX 1000
#define CROSS 1000

/* 
 * 座標(ノード)を表示する関数
 * 引数1: 座標(ノード), 引数2: 座標の個数
 * 返り値: なし               
 */
void outputPoint(point_t* point, int n) {
  int i;
  
  printf("~ point list ~\n");
  for(i = 0; i < n; i++) {
    printf("point %d: x %f y %f, roadA %d, roadB %d\n", point[i].id, point[i].x, point[i].y,
	   point[i].roadA, point[i].roadB);
  }

  return;
}


/* 
 * 道を表示する関数
 * 引数1: 道, 引数2: 道の本数
 * 返り値: なし               
 */
void outputRoad(int** road, int m) {
  int i;
  
  // road test
  printf("~ road list ~\n");
  for(i = 0; i < m; i++) {
    printf("road %d: P %d, Q %d\n", i, road[i][0], road[i][1]);
  }
  
  return;
}

/* 
 * 交差地点を表示する関数
 * 引数1: 交差地点, 引数2: 交差地点の数
 * 返り値: なし               
 */
void outputCrossing(point_t* crossing, int crossCount) {
  int i;

  // crossing test
  printf("~ crossing list ~\n");
  for(i = 0; i < crossCount; i++) {
    printf("crossing %d: %f, %f\n", crossing[i].id, crossing[i].x, crossing[i].y);
  }

  return;
}

/* 
 * 辺を表示する関数
 * 引数1: 辺, 引数2: 座標, 引数3: 道, 引数4: 道の数, 
 * 引数5: 入力座標の数, 引数6: 交点の数 
 * 返り値: なし            
 */
void outputEdge(double** edge, point_t* point, int** road, int m, int n, int crossCount) {
  int i, j;
  int idFrom, idTo, point_id;
  int crossing_id[CROSS];
  int numberOfPoint = n + crossCount;
  
  printf("~ Edge list ~\n");
  for(i = 0; i < m; i++) {
    idFrom = road[i][0];    // 最初は端点Pのid
    for(j = n + 1; j <= numberOfPoint; j++) {
      if( (point[j].roadA == i) || (point[j].roadB == i) ) {
	idTo = point[j].id;
	if(idTo != idFrom) {
	  printf("%d -> %d, ", idFrom, idTo);
	}
	idFrom = idTo;
      }
    }
    idTo = road[i][1];     // 道iの端点Qのid
    printf("%d -> %d\n", idFrom, idTo);
  }

  return;
}
