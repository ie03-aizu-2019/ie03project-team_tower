#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <float.h>
#include "point.h"
#include "pass.h"
#define EPS 0.000001
#define NMAX 1000
#define CROSS 1000
#define INF 10000000



/*
 * 座標のidを受け取り、そのidの座標のインデックスを返す関数
 * 引数1: 座標, 引数2: 全座標の数, 引数3:　座標のid
 * 返り値: 受け取った座標idのインデックス
 */
int searchPointIndex(point_t* point, int numberOfPoint, int id) {
  int i;

  for(i = 1; i <= numberOfPoint; i++) {
    if(point[i].id == id) return i;
  }
  return -1;
}

/*
 * 二つの座標間の距離を返す関数
 * 引数1: 一つ目の座標, 引数2: 二つ目の座標
 * 返り値: 二点間の距離
 */
double calcDistance(point_t pointA, point_t pointB) {
  double distance;

  distance = pow( ((pointA.x - pointB.x)*(pointA.x - pointB.x)
		   + (pointA.y - pointB.y)*(pointA.y - pointB.y)), 0.5 );

  return distance;
}

/* 
 * グラフの辺をつくる関数
 * 引数1: 辺, 引数2: 座標, 引数3: 道, 引数4: 道の数, 
 * 引数5: 入力座標の数, 引数6: 交点の数 
 * 返り値: なし
 */
void generateEdge(double edge[][NMAX], point_t* point,
		  int road[][2], int m, int n, int crossCount) {
  int i, j;
  int numberOfPoint = n + crossCount;
  int idFrom, idTo;

  // idが1の道から辺をつくる
  for(i = 1; i <= m; i++) {
    //　道i上にある交差地点をみつける
    idFrom = road[i][0];    // 最初は端点Pのid
    for(j = n + 1; j <= numberOfPoint; j++) {
      if( (point[j].roadA == i) || (point[j].roadB == i) ) {
	idTo = point[j].id;
	if(idTo != idFrom) {
	  edge[idFrom][idTo] = calcDistance(point[idFrom], point[idTo]);
	  edge[idTo][idFrom] = edge[idFrom][idTo];
	}
	idFrom = idTo;
      }
    }
    // 最後の交点と端点Qの間の辺
    idTo = road[i][1];     // 道iの端点Qのid
    edge[idFrom][idTo] = calcDistance(point[idFrom], point[idTo]);
    edge[idTo][idFrom] = edge[idFrom][idTo];
  }

  return;
}

/* 
 * 最短経路を探索する関数
 * 引数1: 座標(ノード), 引数2: 辺, 引数3:　経路, 引数4: 座標（ノード）の数, 
 * 引数5: スタート座標のid, 引数6: ゴール座標のid, 引数7:　交差地点の数.
 * 返り値: 最短経路のコスト（距離）                
 */
double searchShortestPath(point_t *point, double edge[][NMAX], pass *p, int numberOfPoint, int startid, int goalid, int clossNum) {
  double shortestDistance = 0; 
  point_t processPoint;
  point_t tmpPoint;
	
  int notCrossing = numberOfPoint - clossNum;//交差地点以外の座標の数

  int minCostIndex = 0;
  double newCost;

  int shortestPath[NMAX + CROSS];
  int shortestPathIndex = 0;
  int pathid;
  
  int i, j;
  int indexFori, indexForj;
  int idFori;

  int testCounter = 0;

  // 最後の座標のidよりも大きかったらエラー or 1 より小さかったらエラー
  if( ((startid > point[numberOfPoint].id) || (startid < 1)) || ((goalid > point[numberOfPoint].id) || (goalid < 1)) ) {
    shortestDistance = -1;
    printf("NA\n");
    return shortestDistance;
  }

  // 初期化
  for(i = 1; i <= numberOfPoint; i++) {
    point[i].done = 1;
    point[i].prePointid = INF;  // 同じ距離の場合に辞書順に並べるため
  }

  for(i = 1; i <= numberOfPoint; i++) {
    for(j = 1; j <= numberOfPoint; j++) {
      // 重みつきの辺が存在していたならば
      indexFori = searchPointIndex(point, numberOfPoint, i);
      indexForj = searchPointIndex(point, numberOfPoint, j);
      if(edge[indexFori][indexForj] != 0) {
	point[indexFori].done = 0;
	point[indexForj].done = 0;
      }
    }
  }

  /* ダイクストラ法 */
  // https://www.youtube.com/watch?v=gdmfOwyQlcI
  // 全てのノードのコストを大きな数字で初期化
  for(i = 1; i <= numberOfPoint; i++) {
    point[i].cost = INF;
  }
  // 始点だけはコストが0
  indexFori = searchPointIndex(point, numberOfPoint, startid);
  point[indexFori].cost = 0;
  processPoint = point[indexFori];
  point[indexFori].done++;
  
  while(processPoint.id != goalid) {
    // processNodeにつながるノードのコストを更新
    for(i = 1; i <= numberOfPoint; i++) {
      // 辺が存在していたら(つながっていたら)
      idFori = point[i].id;
      if(edge[processPoint.id][idFori] != 0) {
	newCost = processPoint.cost + edge[processPoint.id][idFori];
	// コストの更新
	indexFori = searchPointIndex(point, numberOfPoint, i);
	if(newCost < point[indexFori].cost) {
 	  point[indexFori].cost = newCost;
	  point[indexFori].prePointid = processPoint.id;
	}
	  // 同じ距離の最短経路が複数あった場合
	if(newCost == point[indexFori].cost) {
	  //test
	  //printf("same %d %d\n", processPoint.id, point[indexFori].prePointid);
	  // 辞書順
	  if(processPoint.id < point[indexFori].prePointid) {
	    point[indexFori].prePointid = processPoint.id;
	  }
	}
      }
    }
    
    // 訪れていないノードの中でコストがもっとも低いノードを訪れる
    tmpPoint.cost = INF;
    for(i = 1; i <= numberOfPoint; i++) {
      indexFori = searchPointIndex(point, numberOfPoint, i);
      if(point[indexFori].done == 0) {
	// 最小のコスト
        if(tmpPoint.cost > point[indexFori].cost) {
	  tmpPoint = point[indexFori];
	  minCostIndex = indexFori;
	}
      }
    }
    processPoint = tmpPoint;
    point[minCostIndex].done++;
  }
  shortestDistance = processPoint.cost;

  // 最短経路の距離表示
  //printf("%f\n", shortestDistance);

  /* 経路を記録 */
  pathid = searchPointIndex(point, numberOfPoint, goalid);
  while(pathid != startid) {
    shortestPath[shortestPathIndex] = point[pathid].id;
    shortestPathIndex++;
    pathid = point[pathid].prePointid;
  }
  // 最後にスタートノードを経由地点に入れる
  shortestPath[shortestPathIndex] = point[pathid].id;
  shortestPathIndex++;

  /* 経路表示  
  for(i = shortestPathIndex - 1; i >= 0; i--) {
	  if(shortestPath[i] > notCrossing) printf("C%d ",shortestPath[i]-notCrossing);
	  else printf("%d ", shortestPath[i]);
  }
  printf("\n");
  */

  //求めた最短経路を、最短経路の構造体のアドレスの場所に格納
  //最短経路の構造体には、<始点 ~ 終点> の順で格納されている
  for(i = shortestPathIndex - 1; i >= 0; i--) {
    p->p[shortestPathIndex - (i + 1)] = shortestPath[i];
  }
  
  //距離と経由地点の数を格納
  p->dist = shortestDistance;
  p->index = shortestPathIndex;
  
  return shortestDistance;
}



/*二つの距離を受け取り、等しければtrueを返す関数
 * 引数1: 距離1, 引数2:　距離2.
 * 返り値: true or false   
 */
bool dist_equals(double a, double b){
  bool result;
  result = ( fabs(a - b) <= DBL_EPSILON * fmax(1,fmax(fabs(a),fabs(b))) );
  return result;
}



/*二つの最短路の構造体を受け取り、等しければtrueを返す関数
 * 引数1: 最短路の構造体1, 引数2: 最短路の構造体2.
 * 返り値: true or false   
 */
bool pass_equals(pass *a, pass *b){
  int i;
  bool result = true;

  if( dist_equals(a->dist, b->dist) ){
    if(a->index == b->index){
      for(i = 0; i < a->index; i++){
	if(a->p[i] == b->p[i]);
	else{
	  result = false;
	  i = a->index;
	}
      }
    }
    else result = false;
  }
  else result = false;

  return result;
}





/* 
 * k番目までの最短経路を探索する関数
 * 引数1: 座標(ノード), 引数2: 辺, 引数3:　座標（ノード）の数, 
 * 引数4: スタート座標のid, 引数5: ゴール座標のid, 引数6:　交差地点の数, 
 * 引数7:　求める最短路の数.
 * 返り値: ステータス
 */
bool k_searchShortestPath(point_t *point, double edgeA[][NMAX], int numberOfPoint, int startid, int goalid, int clossNum, int k){
  int i, j, count, max;
  int A_num = 0; //配列Aの求められた有効な最短路の数。つまり何番目まで求められたか
  int B_num = 0; //配列Bの有効な最短路の数
  int currentId; //現在処理中のノード
  int notCrossing = numberOfPoint - clossNum;//交差地点以外の座標の数
  int P_NMAX = NMAX + CROSS;
  bool notExist, status = false;
  pass A[k], B[k*2];
  point_t pt; //座標配列の複製
  double edgeB[NMAX][NMAX]; //辺配列の複製


  //座標＆辺(コピー版)の初期化
  pt = *point;
  for(i = 0; i < NMAX; i++){
    for(j = 0; j < NMAX; j++){
      edgeB[i][j] = edgeA[i][j];
    }
  }

  //第１最短路を求める
  if(searchShortestPath(&pt,edgeB,&A[0],numberOfPoint,startid,goalid,clossNum) == -1){
    return status;
  }
  else A_num++;

  //第k番目までの最短路を求める
  while(1){    //ループ１開始
    currentId = startid;

    while(currentId != goalid){   //ループ２開始
      //座標＆辺(コピー版)の初期化
      pt = *point;
      for(i = 0; i < NMAX; i++){
	for(j = 0; j < NMAX; j++){
	  edgeB[i][j] = edgeA[i][j];
	}
      }

      //配列Aの経路の中で、処理中のノードからの辺で既に通った辺があれば重みをINFにする
      for(i = 0; i < A_num; i++){
	for(j = 0; j < A[i].index; j++){
	  if(A[i].p[j] == currentId){
	    edgeB[currentId][ (A[i].p[j+1]) ] = INF;
	    j = A[i].index;
	  }
	}
      }

      //更新した辺を用いてダイキストラ法を行う

      //処理中のノードから通れる辺が存在する場合のみ、探索する。
      count = 0;
      for(i = 0; i < NMAX; i++){
	if( (edgeB[currentId][i] != INF) && (edgeB[currentId][i] > EPS) ) count++;
      }
     
      if(count){
	searchShortestPath(&pt,edgeB,&B[B_num],numberOfPoint,startid,goalid,clossNum);

	if(B_num == 0) B_num++;
	else{
	  notExist = true;
	  for(i = 0; i < B_num; i++){
	    if(pass_equals(&B[B_num],&B[i])){
	      notExist = false;
	      i = B_num;
	    }
	  }
	  if(notExist) B_num++;
	}
      }

      //処理中のノードを配列Aの最後の要素を参照して、次のノードにする。
      while(1){
	i = 0;
	if(A[A_num-1].p[i] == currentId){
	  currentId = A[A_num-1].p[++i];
	  break;
	}
	else i++;
      }
    }//ループ２終了

    //配列Bの中で最も距離が短い経路を選び、配列Aに移動
    //もし配列Aの最後の要素と同じ距離の経路しかなかった場合は、配列Aの最後の要素に配列Bの最後の要素を上書き
    if(B_num == 0) break;
    
    max = 0;
    for(i = 0; i < B_num; i++){
      if( (B[max].dist > B[i].dist) || (dist_equals(B[max].dist, B[i].dist)) ){
	max = i;
      }
    }

    if( dist_equals(A[A_num - 1].dist, B[max].dist) ){
      A[A_num - 1] = B[max];
      B_num--;
    }
    else{
      A[A_num] = B[max];
      A_num++;
      B_num--;
    }

    //配列Bを整理する
    if(max != B_num){
      for(i = max; i < B_num; i++){
	B[i] = B[i+1];
      }
    }

    if(A_num == k) break;
      
  }//ループ１終了

  //配列A、すなわち求められた最短路を出力
  for(i = 0; i < A_num; i++){
    printf("%f\n",A[i].dist);
    for(j = 0; j < A[i].index; j++){
      if(A[i].p[j] > notCrossing) printf("C%d ",A[i].p[j] - notCrossing);
      else printf("%d ", A[i].p[j]);
    }
    printf("\n");
  }

  status = true;
  return status;
  
} //k_searchShortestPath 終了





//searchShortestPathで終点に到達できない場合、−１を返すようにしてほしい。
