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
 * 引数1: 辺, 引数2: 座標, 引数3: 道, 引数4: 道の数, 
 * 引数5: 入力座標の数, 引数6: 交点の数 
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
  int indexFori, indexForj;
  int idFori;

  int testCounter = 0;

  // 最後の座標のidよりも大きかったらエラー or 1 より小さかったらエラー
  if( ((startid > point[numberOfPoint].id) || (startid < 1)) || ((goalid > point[numberOfPoint].id) || (goalid < 1)) ) {
    shortestDistance = -1;
    printf("NA\n");
    return shortestDistance;
  }

  // 初期化
  for(i = 1; i <= numberOfPoint; i++) {
    point[i].done = 1;
    point[i].prePointid = INF;  // 同じ距離の場合に辞書順に並べるため
  }

  for(i = 1; i <= numberOfPoint; i++) {
    for(j = 1; j <= numberOfPoint; j++) {
      // 重みつきの辺が存在していたならば
      indexFori = searchPointIndex(point, numberOfPoint, i);
      indexForj = searchPointIndex(point, numberOfPoint, j);
      if(edge[indexFori][indexForj] != 0) {
	point[indexFori].done = 0;
	point[indexForj].done = 0;
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
  indexFori = searchPointIndex(point, numberOfPoint, startid);
  point[indexFori].cost = 0;
  processPoint = point[indexFori];
  point[indexFori].done++;
  
  while(processPoint.id != goalid) {
    // processNodeにつながるノードのコストを更新
    for(i = 1; i <= numberOfPoint; i++) {
      // 辺が存在していたら(つながっていたら)
      idFori = point[i].id;
      if(edge[processPoint.id][idFori] != 0) {
	newCost = processPoint.cost + edge[processPoint.id][idFori];
	// コストの更新
	indexFori = searchPointIndex(point, numberOfPoint, i);
	if(newCost < point[indexFori].cost) {
 	  point[indexFori].cost = newCost;
	  point[indexFori].prePointid = processPoint.id;
	}
	  // 同じ距離の最短経路が複数あった場合
	if(newCost == point[indexFori].cost) {
	  //test
	  //printf("same %d %d\n", processPoint.id, point[indexFori].prePointid);
	  // 辞書順
	  if(processPoint.id < point[indexFori].prePointid) {
	    point[indexFori].prePointid = processPoint.id;
	  }
	}
      }
    }
    
    // 訪れていないノードの中でコストがもっとも低いノードを訪れる
    tmpPoint.cost = INF;
    for(i = 1; i <= numberOfPoint; i++) {
      indexFori = searchPointIndex(point, numberOfPoint, i);
      if(point[indexFori].done == 0) {
	// 最小のコスト
        if(tmpPoint.cost > point[indexFori].cost) {
	  tmpPoint = point[indexFori];
	  minCostIndex = indexFori;
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
  pathid = searchPointIndex(point, numberOfPoint, goalid);
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
