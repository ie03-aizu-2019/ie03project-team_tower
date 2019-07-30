#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "point.h"
#define EPS 0.000001
#define NMAX 1000
#define CROSS 499500
#define INF 10000000
#define WHITE 1
#define GRAY 2
#define BLACK 3

/*
 * 座標のidを受け取り、そのidの座標のインデックスを返す関数
 * 引数1: 座標, 引数2: 全座標の数, 引数3:　座標のid
 * 返り値: 受け取った座標idのインデックス
 */
int searchPointIndex(point_t* point, int numberOfPoint, int id) {
  int i;

  for(i = 0; i < numberOfPoint; i++) {
    if(point[i].id == id) return i;
  }
  return -1;
}


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
 * 引数1: 辺, 引数2: 座標, 引数3: 道, 引数4: 道の数, 
 * 引数5: 入力座標の数, 引数6: 交点の数 
 * 返り値: なし
 */
void generateEdge(double** edge, point_t* point,
		  int** road, int m, int n, int crossCount) {
  int i, j;
  int numberOfPoint = n + crossCount;
  int idFrom, idTo;
  int indexFrom = 0;
  int indexTo = 0;


  printf("~ edge list ~\n");
  // idが1の道から辺をつくる
  for(i = 0; i < m; i++) {
    //　道i上にある交差地点をみつける
    idFrom = road[i][0];    // 最初は端点Pのid
    for(j = n; j < numberOfPoint; j++) {
      if( (point[j].roadA == i + 1) || (point[j].roadB == i + 1) ) {
	idTo = point[j].id;
	if(idTo != idFrom) {
	  indexFrom = searchPointIndex(point, numberOfPoint, idFrom);
	  indexTo = searchPointIndex(point, numberOfPoint, idTo);
	  edge[idFrom][idTo] = calcDistance(point[indexFrom], point[indexTo]);
	  edge[idTo][idFrom] = edge[idFrom][idTo];
	  // テスト出力
	  printf("idFrom: %d, idTo: %d, dist: %lf\n", idFrom, idTo, edge[idFrom][idTo]);
	}
	idFrom = idTo;
      }
    }
    // 最後の交点と端点Qの間の辺
    idTo = road[i][1];     // 道iの端点Qのid
    indexFrom = searchPointIndex(point, numberOfPoint, idFrom);
    indexTo = searchPointIndex(point, numberOfPoint, idTo);
    edge[idFrom][idTo] = calcDistance(point[indexFrom], point[indexTo]);
    edge[idTo][idFrom] = edge[idFrom][idTo];
    // テスト出力
    printf("idFrom: %d, idTo: %d, dist: %lf\n", idFrom, idTo, edge[idFrom][idTo]);
    printf("-------------------\n");
  }

  return;
}

void bridgeDfs(point_t* point, double** edge, int u, int v, int* low, int* pre, int numberOfPoint) {
  static int count;
  int i;
  int indexFori = 0;

  count++;
  pre[v] = count;
  low[v] = pre[v];

  for(i = 1; i <= numberOfPoint; i++) {
      indexFori = searchPointIndex(point, numberOfPoint, i);
      // 全てのidについてやったらbreak
      if(indexFori == -1) break;
      // 全ての隣接するノードについて
      if(edge[v][i] != 0) {
	if(pre[i] == -1) {
	  bridgeDfs(point, edge, v, i, low, pre, numberOfPoint);

	  low[v] = min(low[v], low[i]);
	  if(low[i] == pre[i]) {
	    printf("bridge: %d, %d\n", v, i);
	  }
	} else if(i != u) {
	  low[v] = min(low[v], pre[i]);
	}
      }
  }

  return;
}

int min(int x, int y) {
  if(x > y) return y;
  else return x;
}
