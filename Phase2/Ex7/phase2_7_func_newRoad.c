#include <stdio.h>
#include <math.h>
#include "point.h"
#define MMAX 500
#define INF 100000000

/*
 * 座標のidを受け取り、そのidの座標のインデックスを返す関数
 * 引数1: 座標, 引数2: 全座標の数, 引数3:　座標のid
 * 返り値: 受け取った座標idのインデックス
 * 補足: phase1_4_func_shortest.c内にもあり
 */
int searchPointIndex(point_t* point, int numberOfPoint, int id) {
  int i;

  for(i = 1; i <= numberOfPoint; i++) {
    if(point[i].id == id) return i;
  }
  return -1;
}


/* 
 * 新しい地点をつなげるとき、どこにつなげればいいかを判断する関数
 * 引数1: 座標,  引数2: 道,  引数3: 新しい地点
 * 返り値: 新しい地点から最短の道路網の座標                
 */
point_t suggestNewRoad(point_t *point, int road[][2], int n, int m, point_t newPoint) {
  int i;
  double tmpX;
  double x1, y1, x2, y2, x3, y3;
  double a, b, c;  // 方程式の係数
  double discriminant;  // 判別式
  int pointPIndex, pointQIndex;
  
  double min;
  int minIndex;

  double distance[MMAX + 1];
  double x[MMAX + 1], y[MMAX + 1];

  point_t connectPoint;
  point_t notExist = {-1, -1};

  x3 = newPoint.x;
  y3 = newPoint.y;
  
  /*
   * 全ての道に対して内積を求め、
   * 内積が0の時の座標と距離を出す(その道までの最短距離)
   * そのなかで一番距離が小さいものを採用
   */
  for(i = 1; i <= m; i++) {
    pointPIndex = searchPointIndex(point, n, road[i][0]);
    pointQIndex = searchPointIndex(point, n, road[i][1]);
    // x1, y1 は 線分端点Pの座標
    x1 = point[pointPIndex].x;
    y1 = point[pointPIndex].y;
    // x2, y2 は 線分端点Qの座標
    x2 = point[pointQIndex].x;
    y2 = point[pointQIndex].y;
    
    // 方程式の係数
    a = ( ((x2 - x1)*(x2 - x1)) + ((y2 - y1)*(y2 - y1)) ) / ((x2 - x1) * (x2 - x1));
    b = -1 * ( ( (x2 - x1)*(x3 - x1) + (y2 - y1)*(y3 - y1) ) / (x2 - x1) );
    c = x1 * x3;

    // 判別式
    discriminant = b * b - (4 * a * c);
    if(discriminant < 0) {
      x[i] = INF;
      y[i] = INF;
      distance[i] = INF;
      continue;
    }
    // 方程式を解く
    tmpX = ( -b + sqrt(discriminant) ) / (2 * a);
    printf("tmpX: %f\n", tmpX);
    if((tmpX > x1) && (tmpX < x2)) tmpX = ( -b - sqrt(discriminant) ) / (2 * a);
    printf("tmpX2: %f\n", tmpX);
    x[i] = tmpX;
    y[i] = ( ( (y2 - y1)/(x2 - x1) ) * tmpX ) + y1;
    // 距離を求める
    distance[i] = sqrt(fabs( (x3 - x[i])*(x3 - x[i]) + (y3 - y[i])*(y3 - y[i]) ));
  }

  // test
  for(i = 1; i <= m; i++) {
    printf("distance: %f, x: %f, y: %f\n", distance[i], x[i], y[i]);
  }

  // 最短距離の座標を返す
  min = INF;
  minIndex = 0;
  for(i = 1; i <= m; i++) {
    if(min > distance[i]) {
      min = distance[i];
      minIndex = i;
    }
  }

  connectPoint.x = x[minIndex];
  connectPoint.y = y[minIndex];

  return connectPoint;
}