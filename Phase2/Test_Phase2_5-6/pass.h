//#define NMAX 1000
//#define CROSS 1000

#define NMAX 100
#define CROSS 100

#ifndef PASS_H
#define PASS_H
// 最短経路の構造体
typedef struct {
  double dist;          //距離
  int index;            //経由地点の数
  int p[NMAX + CROSS];  //経路
} pass;

#endif
