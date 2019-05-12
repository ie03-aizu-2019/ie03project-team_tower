#ifndef POINT_H
#define POINT_H
// 座標の構造体
typedef struct {
  double x;
  double y;
  int roadA;
  int roadB;
  double cost;    // コスト
  int done;    // 訪問済みのフラグ
  int preNode; // 最短経路として選択された自身の前のノード
  int id;
} point_t;

#endif

