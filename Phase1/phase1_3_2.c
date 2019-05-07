/* s1250183 */
/* 小課題3の関数部分の実装 */

/* 最短距離を求める関数に始点と終点の座標を渡すために構造体を用意しました。　*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define EPS 0.000001     // 10^(-6)
#define CROSS 1000
#define DIGIT 50

typedef struct {
  double x;
  double y;
  int id;
} t_point;

// 交点の構造体
typedef struct {
  t_point point;
  int michiA;  // 交差している線分Aのid
  int michiB;  // 交差している線分Bのid
} t_crossing;

typedef struct {
  t_point pointP;
  t_point pointQ;
  t_crossing cross[CROSS];  // 線分上の交点
  int id;
} t_road;

typedef struct {
  int isCrossing;   // Point : 0 , Crossing_point : 1 .
  int id;           // 座標、または交点のid.
} p_info;

typedef struct {
  p_info start;
  p_info end;
  int enigma;  // 3番目の入力データはまだ何に使われるかわからない.
} start_end;

t_point detectCrossing (t_road*, int, int);
void sortCrossing (t_crossing*, int);
p_info pointRead(char*);
void Test_start_end(start_end*, int); // 表示テスト用の関数.

int main() {
  int n, m, p, q;
  int pointIdQ, pointIdP;
  int i, j;
  int index = 0;
  char point_c[DIGIT];
  p_info tempPoint;
  t_point tmpCrossing;
  t_crossing crossing[CROSS] = {0};

  t_point *point;
  t_road *michi;
  start_end *s_e;

  int roadid;
  int crossCount[CROSS];  // 各線分の交点の数を保持

  // 座標の数、線分の本数の入力
  scanf("%d %d %d %d", &n, &m, &p, &q);

  point = (t_point*)malloc(sizeof(t_point) * n);
  michi = (t_road*)malloc(sizeof(t_road) * m);
  s_e = (start_end*)malloc(sizeof(start_end) * q);
  

  // 座標を入力し、座標の構造体へ格納
  for(i = 0; i < n; i++) {
    scanf("%lf %lf", &point[i].x, &point[i].y);
    point[i].id = i+1;
  }

  // 二つの線分の端点P, Q を入力し、線分を作って線分の構造体へ格納
  for(i = 0; i < m; i++) {
    scanf("%d %d", &pointIdP, &pointIdQ);
    // x座標が小さいほうを線分の端点Pとする
    if(point[pointIdP - 1].x < point[pointIdQ - 1].x) {
      michi[i].pointP = point[pointIdP - 1];
      michi[i].pointQ = point[pointIdQ - 1];
    } else if(point[pointIdP - 1].x == point[pointIdQ - 1].x) {
      // もしx座標が同じなら、y座標が小さいほうを線分の端点Pとする
      if(point[pointIdP - 1].y < point[pointIdQ - 1].y) {
	michi[i].pointP = point[pointIdP - 1];
	michi[i].pointQ = point[pointIdQ - 1];
      } else {
	michi[i].pointP = point[pointIdQ - 1];
	michi[i].pointQ = point[pointIdP - 1];
      }
    } else {
      michi[i].pointP = point[pointIdQ - 1];
      michi[i].pointQ = point[pointIdP - 1];
    }
    michi[i].id = i+1;
  }

  // 始点と終点のデータを読み取り、始点終点の構造体に格納
  for(i = 0; i < q; i++){
    // "start-point"を読み込んで配列に格納
    scanf("%s",point_c);
    tempPoint = pointRead(point_c);
    s_e[i].start.isCrossing = tempPoint.isCrossing;
    s_e[i].start.id = tempPoint.id;

    // "end-point"を読み込んで配列に格納
    scanf("%s",point_c);
    tempPoint = pointRead(point_c);
    s_e[i].end.isCrossing = tempPoint.isCrossing;
    s_e[i].end.id = tempPoint.id;

    // 3番目のよくわからないデータを読み込む
    scanf("%d",&s_e[i].enigma);
  }

  // 全ての線分の組み合わせについて、交差地点を調べる
  for(i = 0; i < (m - 1); i++) {
    for(j = i+1; j < m; j++) {
      tmpCrossing = detectCrossing(michi, i, j);
      if( (tmpCrossing.x != -1) && (tmpCrossing.y != -1) ){
	crossing[index].point = tmpCrossing;
	crossing[index].michiA = michi[i].id;  // 線分Aの交点である
	crossing[index].michiB = michi[j].id;  // 線分Bの交点である
	index++;
      }
    }
  }

  

  /* ここにソートの実装をする */
  sortCrossing(crossing, index);  // ソートする関数を別に作った

  // 交差地点をそれぞれの線分構造体の交点に格納
  for(i = 0; i < index; i++) {
    roadid = crossing[i].michiA - 1;
    michi[roadid].cross[crossCount[roadid]] = crossing[i];
    crossCount[roadid]++;  // 線分構造体の中の交点の数をアップ
  }

  // 各線分の交点の数を表示するテスト
  for(i = 0; i < m; i++) {
    printf("線分id:%d, 交点の数:%d\n", i+1, crossCount[i]);
  }
  

  // 交差地点の表示
  for(i = 0; i < index; i++) {
    printf("%f %f %d %d\n", crossing[i].point.x, crossing[i].point.y, crossing[i].michiA, crossing[i].michiB);
  }
  
  //始点終点テストの実行
  Test_start_end(s_e,q);
  
  return 0;
}

// 交差地点を返す関数, ない場合はNotExist{-1, -1}を返す
t_point detectCrossing(t_road* michi, int roadNumberA, int roadNumberB) {
  double s, t;
  double x, y;
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

  // 行列式を求める
  determinant = ( (q1X - p1X)*(p2Y - q2Y) + (q2X - p2X)*(q1Y - p1Y) );

  // Step1
  if( (determinant <= EPS) && (determinant >= EPS) ) {
    return notExist;
  }
  
  // Step2
  // パラメータを求める
  s = fabs(((q2Y - p2Y)*(p2X - p1X) - (q2X - p2X)*(p2Y - p1Y))) / determinant;
  t = fabs(((q1Y - p1Y)*(p2X - p1X) - (q1X - p1X)*(p2Y - p1Y))) / determinant;

  // Step3
  // 線分の端点にある場合(s == 0, 1 or t == 0, 1)は交差地点でない
  if( ((s <= EPS)&&(s >= EPS)) || ((t >= EPS)&&(t <= EPS)) ||
      ((s-1 <= EPS)&&(s-1 >= EPS)) || ((t-1 <= EPS)&&(t-1 >= EPS)) ) {
    return notExist;
  }
  
  else if( ((s > 0)&&(s < 1)) && ((t > 0)&&(t < 1))) {
    // Step 4
    // 交差地点を求める
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

// 交差地点をソートする関数
void sortCrossing(t_crossing* crossing, int index) {
  int i, j;
  t_point tmpPoint;

  // バブルソート
  for(i = 0; i < (index - 1); i++) {
    for(j = (index - 1); j > i; j--) {
      // x座標の小さい順に並べる
      if(crossing[j-1].point.x > crossing[j].point.x) {
	tmpPoint = crossing[j-1].point;
	crossing[j-1].point = crossing[j].point;
	crossing[j].point = tmpPoint;
      }
      // x座標が同じ交差地点だった場合
      if( (fabs(crossing[j-1].point.x - crossing[j].point.x) <= EPS) ) {
	// y座標がより小さい順に並べる
	if(crossing[j-1].point.y > crossing[j].point.y) {
	  tmpPoint = crossing[j-1].point;
	  crossing[j-1].point = crossing[j].point;
	  crossing[j].point = tmpPoint;
	}
      }
    }
  }

  // idを更新
  for(i = 0; i < index; i++) {
    crossing[i].point.id = i+1;
  }

  return;
}

//文字型の地点情報を"p_info"型に変換して返す関数
p_info pointRead(char *p){
  p_info info = {0,0};

  if(p[0] == 'C'){
    info.isCrossing = 1;
    info.id = atoi(&p[1]);
  }
  else{
    info.id = atoi(p);
  }
 
  return info;
}

//始点と終点のデータを読み取り、始点終点の構造体に格納できているかの確認
void Test_start_end(start_end* se, int q){
  int i;
  printf("----------Test of Start_End----------\n");
  for(i = 0; i < q; i++){
    printf("<Start_end[%d]>\n Start :",i+1);
    if(se[i].start.isCrossing == 1) printf("C");
    printf("%d\n",se[i].start.id);

    printf(" End   :");
    if(se[i].end.isCrossing == 1) printf("C");
    printf("%d\n",se[i].end.id);

    printf(" Enigma:%d\n\n",se[i].enigma);
  }

  printf("--------------------------------------\n\n");
}
  
