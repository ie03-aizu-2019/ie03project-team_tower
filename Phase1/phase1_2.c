/* 関数内の線分端点判定をシンプルにする */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define EPS 0.000001     // 10^(-6)
#define CROSS 1000

typedef struct {
  double x;
  double y;
  int id;
} t_point;

typedef struct {
  t_point pointQ;
  t_point pointP;
  int id;
} t_road;

t_point detectCrossing (t_road*, int, int);
void sortCrossing (t_point*, int);

int main() {
  int n, m, p, q;
  int pointIdQ, pointIdP;
  int i, j;
  int index = 0;
  t_point tmpCrossing;
  t_point crossing[CROSS] = {0};

  t_point *point;
  t_road *michi;

  // 座標の数、線分の本数の入力
  scanf("%d %d %d %d", &n, &m, &p, &q);

  point = (t_point*)malloc(sizeof(t_point) * n);
  michi = (t_road*)malloc(sizeof(t_road) * m);

  // 座標を入力し、座標の構造体へ格納
  for(i = 0; i < n; i++) {
    scanf("%lf %lf", &point[i].x, &point[i].y);
    point[i].id = i+1;
  }

  // 二つの線分の端点P, Q を入力し、線分を作って線分の構造体へ格納
  for(i = 0; i < m; i++) {
    scanf("%d %d", &pointIdQ, &pointIdP);
    michi[i].pointQ = point[pointIdQ - 1];
    michi[i].pointP = point[pointIdP - 1];
    michi[i].id = i+1;
  }

  // 全ての線分の組み合わせについて、交差地点を調べる
  for(i = 0; i < (m - 1); i++) {
    for(j = i+1; j < m; j++) {
      tmpCrossing = detectCrossing(michi, i, j);
      if( (tmpCrossing.x != -1) && (tmpCrossing.y != -1) ){
	crossing[index] = tmpCrossing;
	index++;
      }
    }
  }

  /* ここにソートの実装をする */
  sortCrossing(crossing, index);  // ソートする関数を別に作った

  // 交差地点の表示
  for(i = 0; i < index; i++) {
    printf("%f %f\n", crossing[i].x, crossing[i].y);
  }
  
  return 0;
}

// 交差地点を返す関数, ない場合はNotExist{-1, -1}を返す
t_point detectCrossing(t_road* michi, int roadNumberA, int roadNumberB) {
  double s, t;
  double x, y;
  double determinant;
  t_point crossing;
  t_point notExist = {-1, -1};
  double p1X, p1Y, q1X, q1Y, p2X, p2Y, q2X, q2Y;
  int i, index;

  p1X = michi[roadNumberA].pointP.x;
  p1Y = michi[roadNumberA].pointP.y;
  q1X = michi[roadNumberA].pointQ.x;
  q1Y = michi[roadNumberA].pointQ.y;
  p2X = michi[roadNumberB].pointP.x;
  p2Y = michi[roadNumberB].pointP.y;
  q2X = michi[roadNumberB].pointQ.x;
  q2Y = michi[roadNumberB].pointQ.y;

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

    return crossing;
  }

  else {
    return notExist;
  }
}

// 交差地点をソートする関数
void sortCrossing(t_point* crossing, int index) {
  int i, j;
  t_point tmpPoint;

  // バブルソート
  for(i = 0; i < (index - 1); i++) {
    for(j = (index - 1); j > i; j--) {
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

  // idを更新
  for(i = 0; i < index; i++) {
    crossing[i].id = i+1;
  }

  return;
}



