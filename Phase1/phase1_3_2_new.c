/* s1250183 */
/* Phase1を作り直す */
/* michiという構造体を無くし、座標だけにする*/
/* 2<= N <= 1000, 1<= M <= 500 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define EPS 0.000001
#define NMAX 1000
#define MMAX 500
#define CROSS 1000
#define DIGIT 50

// 座標の構造体
typedef struct {
  double x;
  double y;
  int roadA;
  int roadB;
  int id;
} point_t;

point_t detectCrossing(point_t, point_t, point_t, point_t);  // 交差地点を出す関数
void sortCrossing(point_t*, int, int);  // 交差地点をxが小さい順にソートする関数

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

// 交差地点を返す関数, ない場合はNotExist{-1, -1}を返す
point_t detectCrossing(point_t pointP_A, point_t pointQ_A, point_t pointP_B, point_t pointQ_B) {
  double p1X, p1Y, q1X, q1Y, p2X, p2Y, q2X, q2Y;
  double s, t;
  double x, y;
  double determinant;
  point_t crossing;
  point_t notExist = {-1, -1};
  int i, index;

  p1X = pointP_A.x;
  p1Y = pointP_A.y;
  q1X = pointQ_A.x;
  q1Y = pointQ_A.y;
  p2X = pointP_B.x;
  p2Y = pointP_B.y;
  q2X = pointQ_B.x;
  q2Y = pointQ_B.y;

  // 行列式を求める
  determinant = ( (q1X - p1X)*(p2Y - q2Y) + (q2X - p2X)*(q1Y - p1Y) );

  // Step1
  if( (determinant <= EPS) && (determinant >= EPS) ) {
    return notExist;
  }
  
  // Step2
  // パラメータを求める
  s = fabs(((q2Y - p2Y)*(p2X - p1X) - (q2X - p2X)*(p2Y - p1Y))) / determinant;
  t = fabs(((q1Y - p1Y)*(p2X - p1X) - (q1X - p1X)*(p2Y - p1Y))) / determinant;

  // Step3
  // 線分の端点にある場合(s == 0, 1 or t == 0, 1)は交差地点でない
  if( ((s <= EPS)&&(s >= EPS)) || ((t >= EPS)&&(t <= EPS)) ||
      ((s-1 <= EPS)&&(s-1 >= EPS)) || ((t-1 <= EPS)&&(t-1 >= EPS)) ) {
    return notExist;
  }
  
  else if( ((s > 0)&&(s < 1)) && ((t > 0)&&(t < 1))) {
    // Step 4
    // 交差地点を求める
    x = p1X + (q1X - p1X) * s;
    y = p1Y + (q1Y - p1Y) * s;

    crossing.x = x;
    crossing.y = y;
    crossing.roadA = pointP_A.roadA;  // この交差地点は線分A と 線分Bが交差したものであることを記録
    crossing.roadB = pointP_B.roadA;

    return crossing;
  }

  else {
    return notExist;
  }
}

// 交差地点をソートする関数
void sortCrossing(point_t* crossing, int crossCount, int n) {
  int i, j;
  point_t tmpPoint;
  int newid;

  // バブルソート
  for(i = 0; i < (crossCount - 1); i++) {
    for(j = (crossCount - 1); j > i; j--) {
      // x座標の小さい順に並べる
      if(crossing[j-1].x > crossing[j].x) {
	tmpPoint = crossing[j-1];
	crossing[j-1] = crossing[j];
	crossing[j] = tmpPoint;
      }
      // x座標が同じ交差地点だった場合
      if( (fabs(crossing[j-1].x - crossing[j].x) <= EPS) ) {
	// y座標がより小さい順に並べる
	if(crossing[j-1].y > crossing[j].y) {
	  tmpPoint = crossing[j-1];
	  crossing[j-1] = crossing[j];
	  crossing[j] = tmpPoint;
	}
      }
    }
  }

  // idを更新(全ての入力した座標のidの後に、交点のidが続く) ex) 1, 2, 3, C1(4), C2(5), ...
  newid = n+1;  // 交点のidは最後の入力した座標のidの次から始まる
  for(i = 0; i < crossCount; i++) {
    crossing[i].id = newid;  // C1, C2, ....
    newid++;
  }

  return;
}
