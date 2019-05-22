#include <stdio.h>
#include <math.h>
#include "point.h"
#define EPS 0.000001
#define NMAX 1000
#define CROSS 1000
#define INF 10000000

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
void generateEdge(double edge[][NMAX], point_t* point,
		  int road[][2], int m, int n, int crossCount) {
  int i, j;
  int numberOfPoint = n + crossCount;
  int idFrom, idTo;
  int crossing_id[CROSS];

  // idが1の道から辺をつくる
  for(i = 1; i <= m; i++) {
    //　道i上にある交差地点をみつける
    idFrom = road[i][0];    // 最初は端点Pのid
    for(j = n + 1; j <= numberOfPoint; j++) {
      // 交差地点の中で、道i上にあるものだったら
      if((point[j].roadA == i) || (point[j].roadB == i)) {
	idTo = j;             // 交点のid
	edge[idFrom][idTo] = calcDistance(point[idFrom], point[idTo]);
	edge[idTo][idFrom] = edge[idFrom][idTo];   // 双方向
	idFrom = idTo;      // 次へ
      }
    }
    // 最後の交点と端点Qの間の辺
    idTo = road[i][1];     // 道iの端点Qのid
    edge[idFrom][idTo] = calcDistance(point[idFrom], point[idTo]);
    edge[idTo][idFrom] = edge[idFrom][idTo];
  }

  return;
}

/* 
 * 最短経路を探索する関数
 * 引数1: 座標(ノード), 引数2: 辺, 引数3: 座標（ノード）の数, 
 * 引数4: スタート座標のid, 引数5: ゴール座標のid
 * 返り値: 最短経路のコスト（距離）                
 */
double searchShortestPath(point_t *point, double edge[][NMAX], int numberOfPoint, int startid, int goalid, int clossNum) {
  double shortestDistance = 0; 
  point_t processPoint;
  point_t tmpPoint;
	
  int notCrossing = numberOfPoint - clossNum;//交差地点以外の座標の数

  int minCostIndex = 0;
  double newCost;

  int shortestPath[NMAX + CROSS];
  int shortestPathIndex = 0;
  int pathid;
  
  int i, j;

  if( ((startid > numberOfPoint) || (startid < 1)) || ((goalid > numberOfPoint) || (goalid < 1)) ) {
    shortestDistance = -1;
    printf("NA\n");
    return shortestDistance;
  }

  // 初期化
  for(i = 1; i <= numberOfPoint; i++) {
    point[i].done = 1;
  }

  for(i = 1; i <= numberOfPoint; i++) {
    for(j = 1; j <= numberOfPoint; j++) {
      // 重みつきの辺が存在していたならば
      if(edge[i][j] != 0) {
	point[i].done = 0;
	point[j].done = 0;
      }
    }
  }

  /* ダイクストラ法 */
  // https://www.youtube.com/watch?v=gdmfOwyQlcI
  // 全てのノードのコストを大きな数字で初期化
  for(i = 1; i <= numberOfPoint; i++) {
    point[i].cost = INF;
  }
  // 始点だけはコストが0
  point[startid].cost = 0;
  processPoint = point[startid];
  point[startid].done++;
  
  while(processPoint.id != goalid) {
    // processNodeにつながるノードのコストを更新
    for(i = 1; i <= numberOfPoint; i++) {
      // 辺が存在していたら(つながっていたら)
      if(edge[processPoint.id][i] != 0) {
	newCost = processPoint.cost + edge[processPoint.id][i];
	// コストの更新(問題)
	if(newCost < point[i].cost) {
	  point[i].cost = newCost;
	  point[i].prePointid = processPoint.id;
	}
      }
    }
    // 訪れていないノードの中でコストがもっとも低いノードを訪れる
    tmpPoint.cost = INF;
    for(i = 1; i <= numberOfPoint; i++) {
      if(point[i].done == 0) {
	// 最小のコスト
        if(tmpPoint.cost > point[i].cost) {
	  tmpPoint = point[i];
	  minCostIndex = i;
	}
      }
    }
    processPoint = tmpPoint;
    point[minCostIndex].done++;
  }
  shortestDistance = processPoint.cost;

  // 最短経路の距離表示
  printf("%f\n", shortestDistance);

  /* 経路を記録 */
  pathid = goalid;
  while(pathid != startid) {
    shortestPath[shortestPathIndex] = point[pathid].id;
    shortestPathIndex++;
    pathid = point[pathid].prePointid;
  }
  // 最後にスタートノードを経由地点に入れる
  shortestPath[shortestPathIndex] = point[pathid].id;
  shortestPathIndex++;

  // 経路表示  ->Fix
  for(i = shortestPathIndex - 1; i >= 0; i--) {
	  if(shortestPath[i] > notCrossing) printf("C%d ",shortestPath[i]-notCrossing);
	  else printf("%d ", shortestPath[i]);
  }
  printf("\n");
  
  return shortestDistance;
}
