/* This is mostly completed */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define EPS 0.000001     // 10^(-6)

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
  double x, y;
  double determinant;
  t_point crossing;
  t_point notExist = {-1, -1};
  double p1X, p1Y, q1X, q1Y, p2X, p2Y, q2X, q2Y;
  int i;

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
  if( (determinant <= EPS) && (determinant >= EPS) ) {
    return notExist;
  }
  
  // Step2
  s = fabs(((q2Y - p2Y)*(p2X - p1X) - (q2X - p2X)*(p2Y - p1Y))) / determinant;
  t = fabs(((q1Y - p1Y)*(p2X - p1X) - (q1X - p1X)*(p2Y - p1Y))) / determinant;

  // Step3
  if( ((s <= EPS)&&(s >= EPS)) || ((t >= EPS)&&(t <= EPS)) ||
      ((s-1 <= EPS)&&(s-1 >= EPS)) || ((t-1 <= EPS)&&(t-1 >= EPS)) ) {
    return notExist;
  }
  
  else if( ((s > 0)&&(s < 1)) && ((t > 0)&&(t < 1))) {
    // Step 4
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

