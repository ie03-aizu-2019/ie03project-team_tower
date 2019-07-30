#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
  // 現在時刻の情報で初期化
  srand((unsigned int)time(NULL));
  
  int n, m, p, q;
  int x, y;
  int pointP, pointQ;
  int i;

  int start, end;
  int orCross;

  n = 100;
  m = 99;
  p = 100;
  q = 100;

  printf("%d %d %d %d\n", n, m, p, q);
  for(i = 0; i < n; i++) {
    // 0から100000までの乱数を発生
    x = rand() % 100000;
    y = rand() % 100000;
    printf("%d %d\n", x, y);
  }
  
  for(i = 0; i < m; i++) {
    // 1から99までの乱数を発生
    pointP = rand() % 99 + 1;
    pointQ = rand() % 99 + 1;
    
    printf("%d %d\n", pointP, pointQ);
  }

  for(i = 0; i < q; i++) {
    // 1から100までの乱数を発生
    start = rand() % 100 + 1;
    end = rand() % 100 + 1;
    
    orCross = rand() % 4;
    if(orCross == 0) {
      printf("%d %d 1\n", start, end);
    } else if(orCross == 1) {
      printf("C%d %d 1\n", start, end);
    } else if(orCross == 2) {
      printf("%d C%d 1\n", start, end);
    } else {
      printf("C%d C%d 1\n", start, end);
    }
  }

  return 0;
}
