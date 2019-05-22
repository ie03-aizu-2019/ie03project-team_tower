/* new version */
#include <math.h>
#include "point.h"
#define EPS 0.000001

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
  determinant = fabs(( (q1X - p1X)*(p2Y - q2Y) + (q2X - p2X)*(q1Y - p1Y) ));

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
void sortCrossing(point_t* crossing, int crossCount, int n) {
  int i, j, k;
  point_t tmpPoint;
  int newid;
  int duplicated = 0;

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
  // idの重複を防ぐ(3本以上の道が交差している時)
  for(i = 0; i < crossCount; i++) {
    for(j = 0; j < crossCount; j++) {
      if( ((crossing[i].x == crossing[j].x)
	   && (crossing[i].y == crossing[j].y)) ) {
	duplicated++;
      }
    }

    if(duplicated > 1) {
      for(k = 0; k < duplicated; k++) {
	crossing[i].id = newid;   // 重複した交差地点は同じid
	i++;
      }
      i--;
    } else {
      crossing[i].id = newid;  // C1, C2, ....
    }
    newid++;
    duplicated = 0;
  }

  return;
}

