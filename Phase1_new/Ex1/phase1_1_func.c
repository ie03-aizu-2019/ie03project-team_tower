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
