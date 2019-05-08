/* s1250183 */
/* 小課題3の関数部分の実装 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define EPS 0.000001     // 10^(-6)
#define CROSS 1000
#define DIGIT 50

// 座標の構造体
typedef struct {
  double x;
  double y;
  int id;
  int michiA;  // 線分Aのid(線分A上の点であることを示す)
  int michiB;  // 線分Bのid(線分B上の点でもあることを示す、交差地点用)
} t_point;

// 線分の構造体
typedef struct {
  t_point pointP;        // 線分の端点P 
  t_point pointQ;        // 線分の端点Q
  t_point cross[CROSS];  // 線分上の交点
  int id;                // 線分のid
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

t_point detectCrossing (t_road*, int, int);  // 交差地点を出す関数
void sortCrossing (t_point*, int, int);      // 交差地点をxが小さい順にソートする関数
p_info pointRead(char*);
void Test_start_end(start_end*, int);        // 表示テスト用の関数.
double calcDistance(t_point, t_point);       // 座標間の距離を求めて返す関数

int main() {
  int n, m, p, q;
  int pointIdP, pointIdQ;
  int i, j;
  int index = 0;
  
  char point_c[DIGIT];
  p_info tempPoint;
  
  t_point tmpCrossing;
  t_point crossing[CROSS] = {0};

  t_point *point;
  t_road *michi;
  start_end *s_e;

  int roadid;
  int crossCount[CROSS];  // 各線分の交点の数を保持

  int edge[CROSS][CROSS] = {0}; // 辺
  int idP;
  int idQ;
  int idCross;
  int idCrossNext;
  int lastCrossIndex;

         /* 入力部分 */
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
    michi[i].pointP.michiA = i+1;   // Pは線分の端点である
    michi[i].pointP.michiB = -1;    // 端点であるため、こっちはなし
    
    michi[i].pointQ.michiA = i+1;   // Qは線分の端点である
    michi[i].pointQ.michiB = -1;    // 端点であるため、こっちはなし
    
    michi[i].id = i+1;   // 線分のidは1からはじまる
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

  /* 交差地点を探し出す部分 */
  // 全ての線分の組み合わせについて、交差地点を調べる
  for(i = 0; i < (m - 1); i++) {
    for(j = i+1; j < m; j++) {
      tmpCrossing = detectCrossing(michi, i, j);
      if( (tmpCrossing.x != -1) && (tmpCrossing.y != -1) ){
	crossing[index] = tmpCrossing;
	index++;
      }
    }
  }

  /* ソート */
  sortCrossing(crossing, index, n);  // ソートする関数を別に作った

  /* 交差地点をそれぞれの線分構造体の交点に格納 */
  for(i = 0; i < index; i++) {
    // 線分Aについて
    roadid = crossing[i].michiA - 1;
    michi[roadid].cross[crossCount[roadid]] = crossing[i];
    crossCount[roadid]++;  // 線分構造体の中の交点の数をアップ
    // 線分Bについて
    roadid = crossing[i].michiB - 1;
    michi[roadid].cross[crossCount[roadid]] = crossing[i];
    crossCount[roadid]++;
  }

  // 各線分の交点の数を表示するテスト
  for(i = 0; i < m; i++) {
    printf("線分id:%d, 交点の数:%d\n", i+1, crossCount[i]);
  }

  /* 辺をつくる */
  // それぞれ線分について、端点Pから交点、そしてQまで辺をつくる
  for(i = 0; i < m; i++) {
    // 辺は重みを持っている（距離）
    roadid = michi[i].id - 1;
    if(crossCount[roadid] != 0) {  // 線分上に交差地点があるなら
      // 端点Pから最初の交差地点までの辺
      idP = michi[i].pointP.id;
      idQ = michi[i].pointQ.id;
      idCross = michi[i].cross[0].id;
      printf("P:%d, Q: %d\n", idP, idQ);
      printf("crossid: %d\n", idCross);
      
      edge[idP][idCross] =
	calcDistance(michi[i].pointP, michi[i].cross[0]);
      // 線分上の交差地点の間の辺をつくる
      lastCrossIndex = crossCount[michi[i].id - 1] - 1;
      for(j = 1; j <= lastCrossIndex; j++) {
	idCross = michi[i].cross[j].id;
	idCrossNext = michi[i].cross[j+1].id;
	edge[idCross][idCrossNext] =
	  calcDistance(michi[i].cross[j], michi[i].cross[j+1]);
      }
      // 最後の交差地点から端点Qまでの辺
      idCross = michi[i].cross[lastCrossIndex].id;
      edge[idCross][idQ] =
	calcDistance(michi[i].cross[lastCrossIndex], michi[i].pointQ);
    }
  }

  // 辺の重みの表示するテスト
  for(i = 0; i < 10; i++) {
    for(j = 0; j < 10; j++) {
      printf("(%d, %d): %f\n", i, j, edge[i][j]);
    }
  }

  /* 交差地点の表示 */
  for(i = 0; i < index; i++) {
    printf("%f %f %d %d\n", crossing[i].x, crossing[i].y, crossing[i].michiA, crossing[i].michiB);
  }

  // 辺の重みの表示するテスト
  for(i = 0; i < 10; i++) {
    for(j = 0; j < 10; j++) {
      printf("(%d, %d): %f\n", i, j, edge[i][j]);
    }
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
  t_point notExist = {-1, -1, -1, -1, -1};
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
    crossing.michiA = roadNumberA + 1;   // この交差地点は線分A と 線分Bが交差したものであること
    crossing.michiB = roadNumberB + 1;   // を記録(線分Aのidと線分Bのid)

    return crossing;
  }

  else {
    return notExist;
  }
}

// 交差地点をソートする関数
void sortCrossing(t_point* crossing, int index, int n) {
  int i, j;
  t_point tmpPoint;
  int newid;

  // バブルソート
  for(i = 0; i < (index - 1); i++) {
    for(j = (index - 1); j > i; j--) {
      // x座標の小さい順に並べる
      if(crossing[j-1].x > crossing[j].x) {
	tmpPoint = crossing[j-1];
	crossing[j-1] = crossing[j];
	crossing[j] = tmpPoint;
      }
      // x座標が同じ交差地点だった場合
      if( (fabs(crossing[j-1].x - crossing[j].x) <= EPS) ) {
	// y座標がより小さい順に並べる
	if(crossing[j-1].y > crossing[j].y) {
	  tmpPoint = crossing[j-1];
	  crossing[j-1] = crossing[j];
	  crossing[j] = tmpPoint;
	}
      }
    }
  }

  // idを更新(全ての入力した座標のidの後に、交点のidが続く) ex) 1, 2, 3, C1(4), C2(5), ...
  newid = n+1;  // 交点のidは最後の入力した座標のidの次から始まる
  for(i = 0; i < index; i++) {
    crossing[i].id = newid;  // C1, C2, ....
    newid++;
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
  
// 指定された座標間の距離を出す
double calcDistance(t_point pointA, t_point pointB) {
  double distance;

  // ピタゴラスの定理
  distance = pow( ((pointA.x - pointB.x)*(pointA.x - pointB.x)
		   + (pointA.y - pointB.y)*(pointA.y - pointB.y)), 0.5 );

  printf("distance: %f\n", distance);

  return distance;
}
