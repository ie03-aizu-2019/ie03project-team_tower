#include <stdio.h>
#include<stdlib.h>
#include <math.h>
#define EPS 0.0000001

typedef struct{
  double x, y;
} coodinate;

coodinate cross_detection(coodinate*);

int main(){
  int x, y, n, m, p, q, i;

  coodinate cross_point;
  coodinate *c;

  scanf("%d%d%d%d",&n,&m,&p,&q);

  c = (coodinate*)malloc(sizeof(coodinate)*n);
  
  /*input*/
  for(i = 0; i < (n + n/2); i++){
    scanf("%lf%lf",&c[i].x, &c[i].y);
  }

  cross_point = cross_detection(c);

  /*output*/
  if( ( (int)(cross_point.x) == -1 ) && ( (int)(cross_point.y) == -1 ) ){
    printf("NA\n");
  }
  else printf("%f %f\n",cross_point.x,cross_point.y);

  return 0;
}

coodinate cross_detection(coodinate *c){
  double A, s, t, xp1, xp2, xq1, xq2, yp1, yp2, yq1, yq2;
  int p1, q1, p2, q2; 
  coodinate cross_point, NA = {-1,-1};

  p1 = ((int)(c[4].x)) - 1;
  q1 = ((int)(c[4].y)) - 1;
  p2 = ((int)(c[5].x)) - 1;
  q2 = ((int)(c[5].y)) - 1;
  
  xp1 = c[p1].x;
  yp1 = c[p1].y;
  xq1 = c[q1].x;
  yq1 = c[q1].y;
  xp2 = c[p2].x;
  yp2 = c[p2].y;
  xq2 = c[q2].x;
  yq2 = c[q2].y;

  A = (xq1-xp1)*(yp2-yq2) + (xq2-xp2)*(yq1-yp1);
  
  /*  step1  */
  if( (A >= EPS) && (A <= EPS) ) return NA;
  
  /*  step2  */
  s = fabs( ( (yq2-yp2)*(xp2-xp1) - (xq2-xp2)*(yp2-yp1) ) ) / A;
  t = fabs( ( (yq1-yp1)*(xp2-xp1) - (xq1-xp1)*(yp2-yp1) ) ) / A;

  /*  step3  */
  if( ( (s <= EPS) && (s >= EPS) ) || ( (t <= EPS) && (t >= EPS) )
   || ( ((s-1) <= EPS) && ((s-1)>= EPS) ) || ( ((t-1) <= EPS) && ((t-1) >= EPS) ))return NA;
  
  else if( ( (s >= 0) && (s <= 1) ) && ( (t >= 0) && (t <= 1) ) ){

  /*  step4  */
    cross_point.x = xp1 + (xq1 - xp1) * s;
    cross_point.y = yp1 + (yq1 - yp1) * s;

    return cross_point;
  }

  else return NA;
}


