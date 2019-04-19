#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define EPS 0.0001

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

t_point detectCrossing(t_road*);

int main() {
  int n, m, p, q;
  int pointIdQ, pointIdP;
  int i;
  t_point crossing;

  t_point *point;
  t_road *michi;

  // input the number of each component
  scanf("%d %d %d %d", &n, &m, &p, &q);

  point = (t_point*)malloc(sizeof(t_point) * n);
  michi = (t_road*)malloc(sizeof(t_road) * m);

  // input coodinates
  for(i = 0; i < n; i++) {
    scanf("%lf %lf", &point[i].x, &point[i].y);
    point[i].id = i+1;
  }

  // input a road with two coodinates
  for(i = 0; i < m; i++) {
    scanf("%d %d", &pointIdQ, &pointIdP);
    michi[i].pointQ = point[pointIdQ - 1];
    michi[i].pointP = point[pointIdP - 1];
    michi[i].id = i+1;
  }

  crossing = detectCrossing(michi);
  if( (crossing.x == -1) && (crossing.y == -1) ) {
    printf("NA\n");
  } else {
    printf("%f %f\n", crossing.x, crossing.y);
  }
  
  return 0;
}

t_point detectCrossing(t_road* michi) {
  double s, t;
  double x1, y1, x2, y2;
  double determinant;
  t_point crossing;
  t_point notExist = {-1, -1};
  double p1X, p1Y, q1X, q1Y, p2X, p2Y, q2X, q2Y;

  p1X = michi[0].pointP.x;
  p1Y = michi[0].pointP.y;
  q1X = michi[0].pointQ.x;
  q1Y = michi[0].pointQ.y;
  p2X = michi[1].pointP.x;
  p2Y = michi[1].pointP.y;
  q2X = michi[1].pointQ.x;
  q2Y = michi[1].pointQ.y;


  determinant = ( (q1X - p1X)*(p2Y - q2Y) + (q2X - p2X)*(q1Y - p1Y) );

  // Step1
  // 誤差を考慮する
  if( (determinant <= EPS) && (determinant >= EPS) ) {
    return notExist;
  }
  
  // Step2
  s = fabs(((q2Y - p2Y)*(p2X - p1X) - (q2X - p2X)*(p2Y - p1Y))) / determinant;
  t = fabs(((q1Y - p1Y)*(p2X - p1X) - (q1X - p1X)*(p2Y - p1Y))) / determinant;

  printf("%f %f\n", s, t);  // test

  // Step3
  if( ((s >= 0)&&(s <= 1)) && ((t >= 0)&&(t <= 1))) {
    // Step 4
    x1 = p1X + (q1X - p1X) * s;
    y1 = p1Y + (q1Y - p1Y) * s;
    x2 = p2X + (q2X - p2X) * t;
    y2 = p2Y + (q2Y - p2Y) * t;

    printf("%f %f %f %f\n", x1, y1, x2, y2);  // test

    // check whether the parameters are correct
    // 誤差を考慮する
    if( ((fabs(x1 - x2) <= EPS) && (fabs(x1 - x2) >= EPS)) &&
	((fabs(y1 - y2) <= EPS) && (fabs(y1 - y2) >= EPS)) ) {
      crossing.x = x1;
      crossing.y = y1;

      return crossing;
    } else {
      return notExist;
    }
    
  } else {
    return notExist;
  }
}

