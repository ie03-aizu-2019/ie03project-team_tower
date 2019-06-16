#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
  // 現在時刻の情報で初期化
  srand((unsigned int)time(NULL));
  
  int n, m, p, q;
  int x, y;
  int i;

  n = 4;
  m = 2;
  p = 0;
  q = 0;

  printf("%d %d %d %d\n", n, m, p, q);
  for(i = 0; i < n; i++) {
    // 0から1000までの乱数を発生
    x = rand() % 1001;
    y = rand() % 1001;
    printf("%d %d\n", x, y);
  }

  return 0;
}
