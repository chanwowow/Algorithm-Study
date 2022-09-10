
#define Max_MAP 1003

#include <unordered_map>
#include <vector>
#include <queue>
#include <iostream>
using namespace std;
// ㅁ 지도
struct piece {
	bool placed;
	int mnt[4];
}MAP[Max_MAP][Max_MAP];

// ㅁ 놓을 곳 후보지
typedef struct myKey { int r; int c; };
struct myVal { int mntN; int rev_mnt[4]; };
unordered_map <int, myVal> pList; // => key는 r*10000, + C로 하자

// ㅁ 정답 후보
struct myStr1 { int mntN; int r; int c; };
struct cmp {
	bool operator()(myStr1 a, myStr1 b) {
		if (a.mntN == b.mntN) {
			if (a.r == b.r) return a.c > b.c;
			return a.r > b.r;
		}
		return a.mntN < b.mntN;
	}
};
priority_queue <myStr1, vector<myStr1>, cmp> ansPQ;
int gM;

int Inv_num(int num, int n) {
	int arr[8] = {};
	int result=0;
	int n1 = n;
	for (int i = n-1; i >=0; i--) {
		arr[i] = num / (int)pow(10, i);
		num -= arr[i] * pow(10, i);
	}
	for (int i = 0; i < n1; i++) {
		result += arr[i] * pow(10, n-- -1);
	}
	return result;
}

int find_mnt(int num, int m) {
	num = Inv_num(num, m);
	int result = 0;
	for (int i = 0; i < m; i++) {
		result += pow(10, i);
	}
	result = result * 10 - num;
	return result;
}

void init(int N, int M, int mU[][4], int mR[][4], int mB[][4], int mL[][4]){
	gM = M;
	// 1. 지도 저장
	for (int i = 1; i <= N; i++) {
		MAP[0][i] = { true,{ mU[i - 1][0], mU[i - 1][1], mU[i - 1][2], mU[i - 1][3] } };
		MAP[i][N + 1] = { true,{ mR[i - 1][0], mR[i - 1][1], mR[i - 1][2], mR[i - 1][3] } };
		MAP[N + 1][i] = { true,{ mB[i - 1][0], mB[i - 1][1], mB[i - 1][2], mB[i - 1][3] } };
		MAP[i][0] = { true,{ mL[i - 1][0], mL[i - 1][1], mL[i - 1][2], mL[i - 1][3] } };
	}

	// 2. pList 저장
	pList[10000 + 1] = { 2,{ find_mnt(mU[0][2], M), 0, 0, find_mnt(mL[0][1] ,M)} };
	pList[10000 + N] = { 2,{ find_mnt(mU[N - 1][2], M), find_mnt(mR[0][3] ,M) ,0,0 } };
	pList[10000 * N + 1] = { 2,{ 0, 0,find_mnt(mB[0][0], M), find_mnt(mL[N - 1][1] ,M)} };
	pList[10000 * N + N] = { 2,{ 0,  find_mnt(mR[N - 1][3] ,M),find_mnt(mB[0][0], M), 0} };
	for (int i = 2; i < N; i++) {
		pList[10000 + i] = { 1, {find_mnt(mU[i - 1][2], M),0,0,0 }};
		pList[10000 * N + i] = { 1, {0, 0, find_mnt(mB[i - 1][0], M),0} };
		pList[10000 * i + 1] = { 1, {0,0,0,find_mnt(mL[i - 1][1], M) } };
		pList[10000 * i + N] = { 1, {0,find_mnt(mR[i - 1][3], M),0,0} };
	}


}

int put(int mPiece[4]){

	bool can = false;
	int result = 0;

	for (auto &itr : pList) {
		int n = itr.second.mntN;
		int i = 0; int j = 0;
		for (i; i < 4; i++) {
			for (j; j < 4; j++) {
				if (mPiece[j] == itr.second.rev_mnt[i]);
				break;
			}
		}

		for (int k = 0; k < 4; k++) {
			if (mPiece[j++] == itr.second.rev_mnt[i++]) n--;
			if (i > 4) i = 0;
			if (j > 4)j = 0;
		}

		if (n <= 0) {
			int r = itr.first / 10000;
			int c = itr.first - r * 10000;
			ansPQ.push({ itr.second.mntN , r, c});

		}
	}
	if (!ansPQ.empty()) {
		can = true;
		myStr1 q = ansPQ.top();
		pList.erase(q.r * 10000 + q.c);
		MAP[q.r][q.c] = {true, }
	}



	return 0;
}

/*
#### int put(int mPiece[4])
1. vector <int> cPiece[4] -> 여기에 저장한다.

bool can =false;
int result =0;
2. pList 전체 돌면서 놓을 수 있는지 판단한다 => 이거 for itr 두면 empty면 자동 탈출되나 실험좀
ㅁ 먼저 놓을 수 있는 면 하나 잡고, vector 전체 돌려서 되는 면 1개 찾고, 나머지 mntN 갯수만큼 다 맞는지 파악

ㅁ되는 놈은 좌표값과, mntN 계산해서 PQ ansList에 저장한다
ㅁ 그놈의 좌표는 u_map에서  삭제한다
ㅁ 그놈의 좌표는 MAP상에는 placed = true 표시하고 저장한다. (cPiece 방향 회전offset 기준 생각해서 넣으셈)
ㅁ 그놈 위치에서 4면 돌면서, MAP상의 placed가 false이고 map에서 검색 안되는놈 있으면, pList에 신규추가
	=> 그위치에서 4면 돌면서 mnt 굴곡 뒤집어서 vector에 저장한다.

if(can) return result;
else return -1;
*/

void destroy(){
}