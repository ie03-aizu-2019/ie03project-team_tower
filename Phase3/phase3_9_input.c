/* new version */
#include <stdio.h>
#include <stdlib.h>
#include "point.h"
#define DIGIT 50

/* 
 * 道の端点PとQを入力する関数
 * 引数1: 座標の数                引数2: 道の数, 
 * 引数3: 追加される地点の座標の数, 引数4: 経路の問い合わせの数
 * 返り値: なし                
 */
void inputNumber(int* n, int* m, int *p, int *q) {
  // n:座標の数, m:道の数, p:追加される地点の座標の数, q: 経路の問い合わせの数
  scanf("%d %d %d %d", n, m, p, q);

  return;
}

void inputPoint(point_t *point, int n) {
  int i;

  for(i = 0; i < n; i++) {
    scanf("%lf %lf", &point[i].x, &point[i].y);
    point[i].id = i + 1;  // 座標のidは1からで、point[0]は使わない
    point[i].roadA = -1;
    point[i].roadB = -1;
    
  }
  
  return;
}

/* 
 * 道の端点PとQを入力する関数
 * 引数1: 座標  引数2: 道, 
 * 引数3: 道の数 
 * 返り値: なし                
 */
void inputRoad(point_t *point, int** road, int m) {
  int i;
  int tmpid;
  int idP, idQ;

  for(i = 0; i < m; i++) {
    scanf("%d %d", &idP, &idQ);
    
    // x座標が小さいほうを線分の端点Pとする
    if(point[idP - 1].x > point[idQ - 1].x) {
      tmpid = idP;
      idP = idQ;
      idQ = tmpid;
    }
    if(point[idP - 1].x == point[idQ - 1].x) {
      // もしx座標が同じなら、y座標が小さいほうを線分の端点Pとする
      if(point[idP - 1].y > point[idQ - 1].y) {
	tmpid = idP;
	idP = idQ;
	idQ = tmpid;
      }
    }
    
    point[idP - 1].roadA = -1;    // 端点はroadAやroadBを使わない
    point[idQ - 1].roadA = -1; 
    point[idP - 1].roadB = -1;
    point[idQ - 1].roadB = -1;

    // 道を作成
    road[i][0] = idP;
    road[i][1] = idQ;
  }

  return;
}

/* 
 * 最短経路の起点と終点を入力する関数
 * 引数1: 起点のidの配列,  引数2: 終点のidの配列, 
 * 引数3: 経路の問い合わせの数, 引数4: 入力した座標の数 
 * 返り値: なし                
 */
void inputPath(int *startid, int *goalid, int q, int n) {
  char point_c[DIGIT];
  int i;
  int notused;

  for(i = 0; i < q; i++) {
    // 経路の起点を入力
    scanf("%s", point_c);
    if(point_c[0] == 'C') startid[i] = n + atoi(&point_c[1]);
    else startid[i] = atoi(point_c);
    // 経路の終点を入力
    scanf("%s", point_c);
    if(point_c[0] == 'C') goalid[i] = n + atoi(&point_c[1]);
    else goalid[i] = atoi(point_c);
    // 追加地点の入力(Phase1_4では使わない)
    scanf("%d", &notused);
  }

  return;
}

/* 
 * 新しい地点を入力し、座標構造体に格納する関数
 * 引数1: 座標, 引数2: 新しい地点の数
 * 返り値: なし                
 */
void inputNewPoint(point_t* newPoint, int p) {
  int i;

  for(i = 0; i < p; i++) {
    scanf("%lf %lf", &newPoint[i].x, &newPoint[i].y);
  }

  return;
}
