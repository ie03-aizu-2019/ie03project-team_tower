#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define EPS 0.000000000001

typedef struct {
  double x;
  double y;
  int id;
} coodinate;

typedef struct {
  coodinate coodQ;
  coodinate coodP;
  int id;
} road;

coodinate detectCrossing(road*);

int main() {
  int n, m, p, q;
  int coodIdQ, coodIdP;
  int i;
  coodinate crossing;

  coodinate *cood;
  road *michi;

  // input the number of each component
  scanf("%d %d %d %d", &n, &m, &p, &q);

  cood = (coodinate*)malloc(sizeof(coodinate) * n);
  michi = (road*)malloc(sizeof(road) * m);

  // input coodinates
  for(i = 0; i < n; i++) {
    scanf("%lf %lf", &cood[i].x, &cood[i].y);
    cood[i].id = i+1;
  }

  // input a road with two coodinates
  for(i = 0; i < m; i++) {
    scanf("%d %d", &coodIdQ, &coodIdP);
    michi[i].coodQ = cood[coodIdQ - 1];
    michi[i].coodP = cood[coodIdP - 1];
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

coodinate detectCrossing(road* michi) {
  double s, t;
  double x1, y1, x2, y2;
  double determinant;
  coodinate crossing;
  coodinate notExist = {-1, -1};
  double p1X, p1Y, q1X, q1Y, p2X, p2Y, q2X, q2Y;

  p1X = michi[0].coodP.x;
  p1Y = michi[0].coodP.y;
  q1X = michi[0].coodQ.x;
  q1Y = michi[0].coodQ.y;
  p2X = michi[1].coodP.x;
  p2Y = michi[1].coodP.y;
  q2X = michi[1].coodQ.x;
  q2Y = michi[1].coodQ.y;


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

