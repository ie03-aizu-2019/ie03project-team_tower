#include <stdio.h>
#include <math.h>
#include "point.h"
#define EPS 0.000001
#define NMAX 1000
#define CROSS 1000

/*
 * 二つの座標間の距離を返す関数
 * 引数1: 一つ目の座標, 引数2: 二つ目の座標
 * 返り値: 二点間の距離
 */
double calcDistance(point_t pointA, point_t pointB) {
  double distance;

  distance = pow( ((pointA.x - pointB.x)*(pointA.x - pointB.x)
		   + (pointA.y - pointB.y)*(pointA.y - pointB.y)), 0.5 );

  return distance;
}


/* 
 * グラフの辺をつくる関数
 * 引数1: 辺, 引数2: 座標, 引数3: 道, 引数4: 道の数, 引数5: 入力座標の数, 引数6: 交点の数 
 * 返り値: なし                
 */
void generateEdge(double edge[][NMAX], point_t* point,
		  int road[][2], int m, int n, int crossCount) {
  int i, j;
  int numberOfPoint = n + crossCount;
  int idFrom, idTo, point_id;
  int crossing_id[CROSS];

  printf("\nEdge list\n");  // test(1)
  // idが1の道から辺をつくる
  for(i = 1; i <= m; i++) {
    //　道i上にある交差地点をみつける
    point_id = road[i][0];  // 道iの端点Pのid
    idFrom = point_id;    // 最初は端点Pのid
    for(j = n + 1; j <= numberOfPoint; j++) {
      // 交差地点の中で、道i上にあるものだったら
      if((point[j].roadA == i) || (point[j].roadB == i)) {
	idTo = j;             // 交点のid
	edge[idFrom][idTo] = calcDistance(point[idFrom], point[idTo]);
	edge[idTo][idFrom] = edge[idFrom][idTo];   // 双方向
	printf("%d -> %d, ", idFrom, idTo);  // test(1)
	idFrom = idTo;      // 次へ
      }
    }
    // 最後の交点と端点Qの間の辺
    idTo = road[i][1];     // 道iの端点Qのid
    edge[idFrom][idTo] = calcDistance(point[idFrom], point[idTo]);
    edge[idTo][idFrom] = edge[idFrom][idTo];
    printf("%d -> %d\n", idFrom, idTo);  // test(1)
  }

  return;
}

/* 
 * 最短経路を探索する関数
 * 引数1: 座標(ノード), 引数2: 辺, 引数3: 座標（ノード）の数
 * 返り値: 最短経路のコスト（距離）                
 */
double searchShortestPath(point_t *point, double edge[][NMAX], int numberOfPoint) {
  double shortestDistance = 0;

  int i;

  // 全てのノードのコストを-1で初期化
  for(i = 1; i <= numberOfPoint; i++) {
    point[i].cost = -1;
  }

  return shortestDistance;
}
