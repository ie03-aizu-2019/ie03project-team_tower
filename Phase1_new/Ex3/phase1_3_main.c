/* new version */
#include <stdio.h>
#include "func.h"
#include "input.h"
#include "point.h"

#define NMAX 1000
#define MMAX 500
#define CROSS 1000

int main() {
  int n, m, p, q;
  int i, j;
  int crossIndex = 0;
  int crossCount = 0;   // 交点の総数

  int road[MMAX+1][2];  // 0: 端点Pのid, 1: 端点Qのid
  int roadA_P, roadA_Q, roadB_P, roadB_Q;  // 道Aの端点Pのid, 道Aの端点Qのid, 道Bも同様

  point_t point[NMAX+1];
  point_t crossing[CROSS];
  point_t tmpPoint;

  /* 入力部分 OK*/
  inputNumber(&n, &m, &p, &q);
  inputPoint(point, n);
  inputRoad(point, road, m);

  printf("\n交差地点を探すときの道の組み合わせ\n");  // 出力テスト(1)
  /* 交差地点を探し出す部分 */
  for(i = 1; i < m; i++) {
    for(j = i + 1; j <= m; j++) {
      roadA_P = road[i][0];
      roadA_Q = road[i][1];
      roadB_P = road[j][0];
      roadB_Q = road[j][1];
      // 出力テスト(1)
      printf("roadA = P:%d Q:%d,  roadB = P:%d Q:%d\n", roadA_P, roadA_Q, roadB_P, roadB_Q);
     

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

  // 出力テスト(2)
  printf("\npointに格納された値(id:1~ )\n");
  for(i = 1; i < crossIndex; i++) {
    printf("x:%f, y:%f\n", point[i].x, point[i].y);
  }
  
  return 0;
}

