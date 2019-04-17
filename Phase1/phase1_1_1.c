#include <stdio.h>
#include <math.h>

typedef struct{
  int x, y;
} coodinate;

int main(){
  int x, y, n, m, p, q, i;

  coodinate *c;

  scanf("%d%d%d%d",&n,&m,&p,&q);

  coodinate = (coodinate*)malloc(sizeof(coodinate)*n);
  
  for(i = 0; i < n; i++){
    scanf("%d%d",&c[i].x, &c[i].y);
  }

  cross_detection(c);
}

double cross_detection(coodinate *c){
  int xp1, xp2, xq1, xq2,yxp1, yp2, yq1, yq2;

  xp1 = c[(c[4].x)-1].x;
  yp1 = c[(c[4].x)-1].y;
  xq1 = c[(c[4].y)-1].x;
  yq1 = c[(c[4].y)-1].y;
  xp2 = c[(c[5].x)-1].x;
  yp2 = c[(c[5].x)-1].y;
  xq2 = c[(c[5].y)-1].x;
  yq2 = c[(c[5].y)-1].y;

  if( ( (xq1-xp1)(yp2-yq2)+(xq2-xp2)(yq1-yp1) ) == 0 ) return -1;

  

}
