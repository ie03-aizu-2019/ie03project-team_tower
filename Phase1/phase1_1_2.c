#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct {
  double x;
  double y;
  int id;
} coodinate;

typedef struct {
  coodinate cood1;
  coodinate cood2;
  int id;
} road;

coodinate detectCrossing(road*);

int main() {
  int n, m, p, q;
  int coodId1, coodId2;
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
    scanf("%d %d", &coodId1, &coodId2);
    michi[i].cood1 = cood[coodId1 - 1];
    michi[i].cood2 = cood[coodId2 - 1];
    michi[i].id = i+1;
  }

  crossing = detectCrossing(michi);
  printf("%f %f\n", crossing.x, crossing.y);
  
  return 0;
}

coodinate detectCrossing(road* michi) {
  double s, t;
  double determinant;
  coodinate crossing;
  coodinate notExist = {-1, -1};
  double p1X, p1Y, q1X, q1Y, p2X, p2Y, q2X, q2Y;

  p1X = michi[0].cood2.x;
  p1Y = michi[0].cood2.y;
  q1X = michi[0].cood1.x;
  q1Y = michi[0].cood1.y;
  p2X = michi[1].cood2.x;
  p2Y = michi[1].cood2.y;
  q2X = michi[1].cood1.x;
  q2Y = michi[1].cood1.y;

  // Meaning  michi[0]:L1, michi[1]:L2, cood1:Q, cood2:P
  determinant = ( (q1X - p1X)*(p2Y - q2Y) + (q2X - p2X)*(q1Y - p1Y) );

  // Step1
  if(determinant == 0) {
    return notExist;
  }

  // --------------- OK ------------------
  // Step2
  s = ((q2Y - p2Y)*(p2X - p1X) - (q2X - p2X)*(p2Y - p1Y)) / determinant;
  
  t = ((q1Y - p1Y)*(p2X - q1X) - (q1X - p1X)*(p2Y - p1Y)) / determinant;

  printf("%f %f\n", s, t);

  // Step3
  if( ((s >= 0)&&(s <= 1)) && ((t >= 0)&&(t <= 1))) {
    // Step 4
    crossing.x = p1X + (q1X - p1X) * s;
    crossing.y = p1Y + (q1Y - p1Y) * s;
  } else {
    return notExist;
  }

  return crossing;
}

