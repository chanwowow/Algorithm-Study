
#define Max_MAP 1003

#include <unordered_map>
#include <vector>
#include <queue>
#include <iostream>
using namespace std;
// �� ����
struct piece {
	bool placed;
	int mnt[4];
}MAP[Max_MAP][Max_MAP];

// �� ���� �� �ĺ���
typedef struct myKey { int r; int c; };
struct myVal { int mntN; int rev_mnt[4]; };
unordered_map <int, myVal> pList; // => key�� r*10000, + C�� ����

// �� ���� �ĺ�
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
	// 1. ���� ����
	for (int i = 1; i <= N; i++) {
		MAP[0][i] = { true,{ mU[i - 1][0], mU[i - 1][1], mU[i - 1][2], mU[i - 1][3] } };
		MAP[i][N + 1] = { true,{ mR[i - 1][0], mR[i - 1][1], mR[i - 1][2], mR[i - 1][3] } };
		MAP[N + 1][i] = { true,{ mB[i - 1][0], mB[i - 1][1], mB[i - 1][2], mB[i - 1][3] } };
		MAP[i][0] = { true,{ mL[i - 1][0], mL[i - 1][1], mL[i - 1][2], mL[i - 1][3] } };
	}

	// 2. pList ����
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
1. vector <int> cPiece[4] -> ���⿡ �����Ѵ�.

bool can =false;
int result =0;
2. pList ��ü ���鼭 ���� �� �ִ��� �Ǵ��Ѵ� => �̰� for itr �θ� empty�� �ڵ� Ż��ǳ� ������
�� ���� ���� �� �ִ� �� �ϳ� ���, vector ��ü ������ �Ǵ� �� 1�� ã��, ������ mntN ������ŭ �� �´��� �ľ�

���Ǵ� ���� ��ǥ����, mntN ����ؼ� PQ ansList�� �����Ѵ�
�� �׳��� ��ǥ�� u_map����  �����Ѵ�
�� �׳��� ��ǥ�� MAP�󿡴� placed = true ǥ���ϰ� �����Ѵ�. (cPiece ���� ȸ��offset ���� �����ؼ� ������)
�� �׳� ��ġ���� 4�� ���鼭, MAP���� placed�� false�̰� map���� �˻� �ȵǴ³� ������, pList�� �ű��߰�
	=> ����ġ���� 4�� ���鼭 mnt ���� ����� vector�� �����Ѵ�.

if(can) return result;
else return -1;
*/

void destroy(){
}