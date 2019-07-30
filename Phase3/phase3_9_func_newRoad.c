#include <stdio.h>
#include <math.h>
#include "point.h"
#define MMAX 199
#define INF 100000000

/* 
 * 新しい地点をつなげるとき、どこにつなげればいいかを判断する関数
 * 引数1: 座標,  引数2: 道,  引数3: 新しい地点
 * 返り値: 新しい地点から最短の道路網の座標                
 */
point_t suggestNewRoad(point_t *point, int** road, int n, int m, point_t newPoint) {
  int i;
  double x1, y1, x2, y2, x3, y3;
  int pointPIndex, pointQIndex;

  double coef1, coef2;
  double min;
  int minIndex;

  double distance[MMAX];
  double x[MMAX], y[MMAX];

  point_t connectPoint;
  point_t notExist = {-1, -1};

  x3 = newPoint.x;
  y3 = newPoint.y;
  
  /*
   * 全ての道に対して内積を求め、
   * 内積が0の時の座標と距離を出す(その道までの最短距離)
   * そのなかで一番距離が小さいものを採用
   */
  for(i = 0; i < m; i++) {
    pointPIndex = searchPointIndex(point, n, road[i][0]);
    pointQIndex = searchPointIndex(point, n, road[i][1]);
    // x1, y1 は 線分端点Pの座標
    x1 = point[pointPIndex].x;
    y1 = point[pointPIndex].y;
    // x2, y2 は 線分端点Qの座標
    x2 = point[pointQIndex].x;
    y2 = point[pointQIndex].y;

    coef1 = pow(x2-x1, 2.0);
    coef2 = pow(y2-y1, 2.0);
    
    // 方程式を解く
    x[i] = (1 / (coef1 + coef2)) * ( coef1 * x3 + coef2 * x1 - (x2-x1)*(y2-y1)*(y1-y3) );
    y[i] = ( ( (y2 - y1)/(x2 - x1) ) * (x[i] - x1) ) + y1;
    // 距離を求める
    distance[i] = sqrt(fabs( (x3 - x[i])*(x3 - x[i]) + (y3 - y[i])*(y3 - y[i]) ));
  }

  /*
  // test
  for(i = 1; i <= m; i++) {
    printf("distance: %f, x: %f, y: %f\n", distance[i], x[i], y[i]);
  }
  */

  // 最短距離の座標を返す
  min = INF;
  minIndex = 0;
  for(i = 0; i < m; i++) {
    if( (min > distance[i]) && (x[i] != x3) && (y[i] != y3) ) {
      min = distance[i];
      minIndex = i;
    }
  }

  connectPoint.x = x[minIndex];
  connectPoint.y = y[minIndex];

  return connectPoint;
}
