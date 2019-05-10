#include <stdio.h>
#include "func.h"
#include "point.h"

#define NMAX 1000
#define MMAX 500
#define CROSS 1000

int main() {
  int n, m, p, q;
  int idP, idQ, tmpid;
  int i, j;
  int crossIndex = 0;
  int crossCount = 0;   // 交点の総数

  int road[MMAX+1][2];  // 0: 端点Pのid, 1: 端点Qのid
  int roadA_P, roadA_Q, roadB_P, roadB_Q;  // 道Aの端点Pのid, 道Aの端点Qのid, 道Bも同様

  point_t point[NMAX+1];
  point_t crossing[CROSS];
  point_t tmpPoint;

  /* 入力部分 */
  // n:座標の数, m:道の数, p:追加される地点の座標の数, q: 経路の問い合わせの数
  scanf("%d %d %d %d", &n, &m, &p, &q);

  // 座標を入力し、座標の構造体へ格納
  for(i = 1; i <= n; i++) {
    scanf("%lf %lf", &point[i].x, &point[i].y);
    point[i].id = i;     // 座標のidは1からで、point[0]は使わない
  }

  // 二つの線分の端点P, Qを入力し、P, Qそれぞれに線分の印をつける
  for(i = 1; i <= m; i++) {
    scanf("%d %d", &idP, &idQ);
    // x座標が小さいほうを線分の端点Pとする
    if(point[idP].x > point[idQ].x) {
      tmpid = idP;
      idP = idQ;
      idQ = tmpid;
    }
    if(point[idP].x == point[idQ].x) {
      // もしx座標が同じなら、y座標が小さいほうを線分の端点Pとする
      if(point[idP].y > point[idQ].y) {
	tmpid = idP;
	idP = idQ;
	idQ = tmpid;
      }
    }
    // 道のidは1から
    point[idP].roadA = i;   // Pは 線分id:i の端点
    point[idQ].roadA = i;   // Qは 線分id:i の端点
    point[idP].roadB = -1;    // Pは端点なので、一本の道にしかのっていない
    point[idQ].roadB = -1;    // Qは端点なので、一本の道にしかのっていない
    // 道を作成
    road[i+1][0] = idP;
    road[i+1][1] = idQ;
  }

  /* 交差地点を探し出す部分 */
  for(i = 1; i < m; i++) {
    for(j = i + 1; j <= m; j++) {
      roadA_P = road[i][0];
      roadA_Q = road[i][1];
      roadB_P = road[j][0];
      roadB_Q = road[j][1];

      tmpPoint =
	detectCrossing(point[roadA_P], point[roadA_Q], point[roadB_P], point[roadB_Q]);
      if( (tmpPoint.x != -1) && (tmpPoint.y != -1) ) {
	crossing[crossCount] = tmpPoint;
	crossCount++;
      }
    }
  }

  /* xが小さい順にソート */
  sortCrossing(crossing, crossCount, n);

  // 交差地点を座標の構造体配列へ格納
  crossIndex = crossing[0].id;
  for(i = 0; i < crossCount; i++) {
    point[crossIndex] = crossing[i];
    crossIndex++;
  }

  // 出力テスト
  for(i = 1; i < crossIndex; i++) {
    printf("x:%f, y:%f\n", point[i].x, point[i].y);
  }
  
  return 0;
}

