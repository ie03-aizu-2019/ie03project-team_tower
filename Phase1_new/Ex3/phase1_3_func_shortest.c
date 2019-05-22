#include <stdio.h>
#include <math.h>
#include "point.h"
#define EPS 0.000001
#define NMAX 1000
#define CROSS 1000
#define INF 10000000

/*
 * 座標のidを受け取り、そのidの座標のインデックスを返す関数
 * 引数1: 座標, 引数2: 全座標の数, 引数3:　座標のid
 * 返り値: 受け取った座標idのインデックス
 */
int searchPointIndex(point_t* point, int numberOfPoint, int id) {
  int i;

  for(i = 1; i <= numberOfPoint; i++) {
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
 * 引数1: 辺, 引数2: 座標, 引数3: 道, 引数4: 道の数, 引数5: 入力座標の数, 引数6: 交点の数 
 * 返り値: なし                
 */
void generateEdge(double edge[][NMAX], point_t* point,
		  int road[][2], int m, int n, int crossCount) {
  int i, j;
  int numberOfPoint = n + crossCount;
  int idFrom, idTo;

  // idが1の道から辺をつくる
  for(i = 1; i <= m; i++) {
    //　道i上にある交差地点をみつける
    idFrom = road[i][0];    // 最初は端点Pのid
    for(j = n + 1; j <= numberOfPoint; j++) {
      if( (point[j].roadA == i) || (point[j].roadB == i) ) {
	idTo = point[j].id;
	if(idTo != idFrom) {
	  edge[idFrom][idTo] = calcDistance(point[idFrom], point[idTo]);
	  edge[idTo][idFrom] = edge[idFrom][idTo];
	}
	idFrom = idTo;
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
 * 引数1: 座標(ノード), 引数2: 辺, 引数3: 座標（ノード）の数
 * 返り値: 最短経路のコスト（距離）                
 */
double searchShortestPath(point_t *point, double edge[][NMAX], int numberOfPoint, int startid, int goalid) {
  double shortestDistance = 0; 
  point_t processPoint;
  point_t tmpPoint;

  int minCostIndex = 0;
  double newCost;
  
  int i, j;
  int testCounter = 0;

  if( ((startid > numberOfPoint) || (startid < 1)) || ((goalid > numberOfPoint) || (goalid < 1)) ) {
    shortestDistance = -1;
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

  // ダイクストラ法
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
	if(newCost < point[i].cost) point[i].cost = newCost;
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
  
  return shortestDistance;
}
