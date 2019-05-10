/* new version */
#include <stdio.h>
#include "point.h"

void inputNumber(int* n, int* m, int *p, int *q) {
  // n:座標の数, m:道の数, p:追加される地点の座標の数, q: 経路の問い合わせの数
  scanf("%d %d %d %d", n, m, p, q);

  return;
}

void inputPoint(point_t *point, int n) {
  int i;

  for(i = 1; i <= n; i++) {
    scanf("%lf %lf", &point[i].x, &point[i].y);
    point[i].id = i;  // 座標のidは1からで、point[0]は使わない
  }
  
  return;
}

void inputRoad(point_t *point, int road[][2], int* idP, int* idQ, int m) {
  int i;
  int tmpid;

  for(i = 1; i <= m; i++) {
    scanf("%d %d", idP, idQ);
    // x座標が小さいほうを線分の端点Pとする
    if(point[*idP].x == point[*idQ].x) {
      tmpid = *idP;
      *idP = *idQ;
      *idQ = tmpid;
    }
    if(point[*idP].x == point[*idQ].x) {
      // もしx座標が同じなら、y座標が小さいほうを線分の端点Pとする
      if(point[*idP].y > point[*idQ].y) {
	tmpid = *idP;
	*idP = *idQ;
	*idQ = tmpid;
      }
    }

    // 道のidは1から
    point[*idP].roadA = i;   // Pは 線分id:i の端点
    point[*idQ].roadA = i;   // Qは 線分id:i の端点
    point[*idP].roadB = -1;    // Pは端点なので、一本の道にしかのっていない
    point[*idQ].roadB = -1;    // Qは端点なので、一本の道にしかのっていない

    // 道を作成
    road[i][0] = *idP;
    road[i][1] = *idQ;
  }

  return;
}
