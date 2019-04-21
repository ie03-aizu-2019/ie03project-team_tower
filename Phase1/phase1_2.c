/* テストケースでid2と3の道路の交差地点はNAであるはずだが、7.0,1.0となる */

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

t_point detectCrossing(t_road*, int, int);

int main() {
  int n, m, p, q;
  int pointIdQ, pointIdP;
  int i, j;
  int index = 0;
  t_point crossing[CROSS] = {0};

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

  for(i = 0; i < (m - 1); i++) {
    for(j = i+1; j < m; j++) {
      crossing[index] = detectCrossing(michi, i, j);
      index++;
    }
  }

  for(i = 0; i < index; i++) {
    if( (crossing[i].x == -1) && (crossing[i].y == -1) ) {
      printf("");
    } else {
      printf("%f %f\n", crossing[i].x, crossing[i].y);
    }
  }
  
  return 0;
}

t_point detectCrossing(t_road* michi, int roadNumberA, int roadNumberB) {
  double s, t;
  double x1, y1, x2, y2;
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

  determinant = ( (q1X - p1X)*(p2Y - q2Y) + (q2X - p2X)*(q1Y - p1Y) );

  // Step1
  if( (determinant <= EPS) && (determinant >= EPS) ) {
    return notExist;
  }
  
  // Step2
  s = fabs(((q2Y - p2Y)*(p2X - p1X) - (q2X - p2X)*(p2Y - p1Y))) / determinant;
  t = fabs(((q1Y - p1Y)*(p2X - p1X) - (q1X - p1X)*(p2Y - p1Y))) / determinant;

  // Step3
  if( ((s >= 0)&&(s <= 1)) && ((t >= 0)&&(t <= 1))) {
    // Step 4
    x1 = p1X + (q1X - p1X) * s;
    y1 = p1Y + (q1Y - p1Y) * s;
    x2 = p2X + (q2X - p2X) * t;
    y2 = p2Y + (q2Y - p2Y) * t;

    // Check whether the crossing point is correct using parameter
    if( (fabs(x1 - x2) <= EPS) && (fabs(y1 - y2) <= EPS) ) {

      index = roadNumberA;
      for(i = 0; i < 2; i++) {
	// Is the point a part of the line edges?
	if( (fabs(x1 - michi[index].pointP.x) <= EPS) || (fabs(x1 - michi[index].pointQ.x) <= EPS)
	    || (fabs(y1 - michi[index].pointP.y) <= EPS) || (fabs(y1 - michi[index].pointQ.y) <= EPS) ) {
	  return notExist;
	}
	index = roadNumberB;
      }
      crossing.x = x1;
      crossing.y = y1;

      return crossing;
    }
  }
  return notExist;
}

